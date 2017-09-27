##
''' This testbed tests the validity or running 3 DES in seperate cores as
    a pipelined application of Triple-DES each taking a 8 byte chunk of data
    from a queue doing a DES encrpyt or decrypt and puting it's 8 byte
    result in another queue to encrypt triple DES 3 You DES encrypt using key1
    then the results you DES decrypt with key2 and the results the DES
    encrypt using key3.
'''

import multiprocessing
import time
import pyDes

#CHUNKS will normally be 10000 However for debug it can be set as low as 1.
# beween loading and envoking Main()
CHUNKS = 10000

def Generatebuf (): # test data is all bytes 0x00..0xFF This is all we need to generate
    global buf      # as it would just repeat after 256
    buf = []
    b = bytearray(8)
    data = 0
    for i in range(32):
        for j in range(8):
            b[j] = data
            data = (data+1) & 0xff
        buf.append(b[:])

def DesRun (key, op, in_q, out_q):
    # get an des object which can encrypt or decrypt
    des = pyDes.des(key, pyDes.CBC, b'\0\0\0\0\0\0\0\0', pad=None, padmode=pyDes.PAD_NORMAL)
    while True:     # loop till we dequeue a None type
        msg = in_q.get()    #block here is no data. If data dequeue it
        if msg == None:     # is it a stop message
            out_q.put(None)     #if so pass it on
            time.sleep(0.25)    #wait for it (the Stop) to percolate thru the system
            return              # and stop by returning from the target.
        if op == 'E':           # 'E' for encrypt and 'D' to decrypt
            result = des.encrypt(msg)
        else:
            result = des.decrypt(msg)
        out_q.put(result)       # which ever we did pipeline it to next stage

def MultiCore3Des ():
    global finalresults
    starttime = time.time()     # timing starts here
    # set up some processes all hibernating
    p1 = multiprocessing.Process(target=DesRun, args=(b'alpha987', 'E', q1, q2))
    p2 = multiprocessing.Process(target=DesRun, args=(b'beta6543', 'D', q2, q3))
    p3 = multiprocessing.Process(target=DesRun, args=(b'gamma210', 'E', q3, qr))
    # wake p1,p2 and p3 up
    p1.start()
    p2.start()
    p3.start()

    finalresults = []   # final ciphertext collected here
    i,j = 0,0           #i packets sent j packets received
    flag = True         # To ensure only one put of None to the output queue takes place
    cipherText = 0      #Just an 8 byte block of ciphertext
    while True:
        # write to q1 if data to send and we have not yet colected 20 in queue
        if (i < CHUNKS) and (q1.qsize() < 20):
            q1.put(buf[i & 0x1f])
            i += 1
        # sent it all, Now queue up None (Stop signal) the signal to terminate
        if flag and (j == CHUNKS):
            q1.put(None)
            flag = False
        # while results are back from q3 stored in qr, retrieve them
        while qr.qsize() > 0:
            cipherText = qr.get()
            j += 1
            if cipherText == None:  #This is all done
                break               #escape inside loop
            finalresults.append(cipherText)
        if cipherText == None:  #till the terminate signal percolates through
            break               #and escape outside loop
    took = time.time() - starttime
    print("Using Multi Core", round(took, 3))
    # Waiting for all 3 DES stages to terminate
    p1.join()
    p2.join()
    p3.join()
    return took

def SingleCoreDes ():
    global test
    des = pyDes.triple_des(b'alpha987beta6543gamma210', pyDes.CBC, b'\0\0\0\0\0\0\0\0', pad=None, padmode=pyDes.PAD_NORMAL)

    starttime = time.time()
    test = []
    for i in range(CHUNKS): #encrypting the same data with a Triple Des object; again 8 bytes at a time
        test.append(des.encrypt(buf[i & 0x1f]))
    took = time.time() - starttime
    print("Using Single Core", round(took, 3))
    return took

def ResultsSame ():     # compare Single core Triple DES results with Multi core results.
    for i in range(CHUNKS):     #we expect finalresults to be equal to test
        if finalresults[i] != test[i]:
            print ("difference in block i", i)
            print ("Multi", finalresults[i])
            print ("Single", test[i])
            return False
    return True

def Setup ():   # makes it easier to start a second time. as old q1...qr are gc'd
    global q1, q2, q3, qr, buf, finalresults, test
    q1 = multiprocessing.Queue()
    q2 = multiprocessing.Queue()
    q3 = multiprocessing.Queue()
    qr = multiprocessing.Queue()

    buf = []
    finalresults = []
    test = []

def Main ():
    print('be fore warned the default of 10000 8 byte blocks takes a while')
    Setup()
    Generatebuf()
    stime = SingleCoreDes()
    mtime = MultiCore3Des()

    if ResultsSame():
        print ("Multi matches Single")
    else:
        print ("Oh No, does not match")
    if mtime >= stime:
        if stime > 0:
            print ("Multicore time/SingleCore time is", round((mtime/stime),3))
        else:
            print ("stime is zero no ratio available")
    else:
        if mtime > 0:
            print("SingleCore time / MultiCore time is", round((stime/mtime),3))
        else:
            print("mtime is zero no ratio available")
