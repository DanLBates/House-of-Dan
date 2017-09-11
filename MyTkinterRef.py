"""
My own experimentation. Having learned how to do some (not to apparent)
tkinter functionality I do not want to forget how. This reference shows me what
I did so I can, when necessary do it again.
"""
import sys
from tkinter import *
from tkinter import messagebox
from tkinter import colorchooser
from tkinter import filedialog

import tkinter.messagebox as messagebox

import time

okvisible = True
trackMouse = False

def toggleTrack (event):
    global trackMouse
    trackMouse = not trackMouse
    if trackMouse:
        print("Mouse Tracking On")
    else:
        print("Mouse Tracking Off")

def motion (event):
    if trackMouse:
        x = root.winfo_pointerx()
        y = root.winfo_pointery()
        print(f"({x},{y})({event.x},{event.y})")

def CanvasXY (event):
    x = theCanvas.winfo_pointerx() - theCanvas.winfo_rootx()
    y = theCanvas.winfo_pointery() - theCanvas.winfo_rooty()
    xy_str.set("x=" + str(x) + " , " + "y=" + str(y))

def CanvasMouseButton (event):
    x = theCanvas.winfo_pointerx() - theCanvas.winfo_rootx()
    y = theCanvas.winfo_pointery() - theCanvas.winfo_rooty()
    print("Left Mouse Btn on Canvas", f"({x},{y})")

def BtnQuit (event):
    mQuit()

def DumpClick (event):
    fmdump()

def Key (event):
    char = event.char
    key = repr(char)
    if char != '':
        print("Key pressed", key, len(key))
    else:
        print('Special keysym_num', event.keysym_num, event.keysym)

def mhello():
    global okvisible
    okvisible = not okvisible
    if okvisible:
        ml = Label(text="Hello World", fg = 'black', bg = 'white')
    else:
        ml = Label(text="                    ", fg="white", bg="white")
    ml.grid(row=0, column=4, sticky=W)
    mtext = ment.get()
    labelt = Label(root, text=mtext, width=10, justify="left")
    labelt.grid(row=0,column=5, sticky=W)

def fmdump():
    print("New in the File Menu selected")
    print("jack =",jack.get())
    print("jill =",jill.get())
    print("radio1 =", radiovar1.get())
    print("radio2 =", radiovar2.get())
    print("spinvar =", spinvar.get())
    thelist = list(List1.curselection())
    print("Listbox =", thelist)
    print("slider =", slidervar.get())
    print("checkbtnvar =", checkbtnvar.get())

def mQuit ():
    mexit = messagebox.askyesno(title="Quit?", message="Are you sure?")
    if mexit > 0:
        root.destroy()

def mAbout ():
    messagebox.showinfo(title="About", message="This is my About Box")

def mColor ():
    myColor = colorchooser.askcolor()
    print(myColor)

def mOpen ():
    myopen = filedialog.askopenfile()
    print("FILE:", myopen)

def BusyProceed ():
    global busy
    newval = not busy.get()
    busy.set(newval)
    if newval:
        print("Busy")
        savebg = busypro['background']
        busyprostr.set(" Busy  ")
        busypro['background'] = "Red"
        savestate = busypro['state']
        busypro['state'] = DISABLED
        root.config(cursor="wait")
        root.update()
        time.sleep(5)
        root.config(cursor="")
        busy.set(False)
        busypro['state'] = savestate
        busypro["background"] = savebg
        busyprostr.set("Proceed")
        print("Proceed")

root = Tk()
ment = StringVar()
jack = BooleanVar()
jill = BooleanVar()
radiovar1 = IntVar()
radiovar2 = IntVar()
radiovar1.set(1)
radiovar2.set(2)
spinvar = StringVar()
spinvar.set('5')
slidervar = IntVar()
slidervar.set(25)
checkbtnvar = BooleanVar()  #could be IntVar
checkbtnvar.set(False)
busy = BooleanVar()
busy.set(False)
busyprostr = StringVar()
xy_str = StringVar()
xy_str.set("None")

root.geometry('750x450+500+200')
root.title('My Tkinter Reference')

# Labels pt 2

label1 = Label(text='My First Label', fg='white', bg='blue')
#label1.pack()
label1.grid(row=0, column=0, sticky=W)

label2 = Label(text='type here!', takefocus=1, fg='blue', bg='red',
               highlightcolor='green', highlightbackground='red',
               borderwidth=4)
label2.tk_focusFollowsMouse()

#label2.pack()
label2.grid(row=0, column=1, sticky=E)

xy_label = Label(textvariable=xy_str, fg="black", bg="white")
xy_label.grid(row=4, column=5)

# Mouse

root.bind("<Motion>", motion)
root.bind("<Double-2>", toggleTrack)
root.bind("<Double-3>", BtnQuit)
root.bind("<Button-3>", DumpClick)

mhello()

hellobutton = Button(text='OK', command=mhello)
hellobutton.grid(row=2, column=4)

mEntry = Entry(root, textvariable = ment)
mEntry.grid(row=2,column=5)

# Menu Construction
menubar = Menu(root)

# File Menu
filemenu = Menu(menubar, tearoff=1)
filemenu.add_command(label = "New")
filemenu.add_command(label = "Open",  command=mOpen)
filemenu.add_command(label = "Color", command=mColor)
filemenu.add_command(label = "Save As")
filemenu.add_command(label = "Close", command = mQuit)
filemenu.add_command(label = "Dump Vars", command = fmdump)
menubar.add_cascade(label  = "File", menu=filemenu)

# SetUp menu
setupmenu = Menu(menubar, tearoff=1)
setupmenu.add_checkbutton(label = "Jack present", onvalue = True, offvalue = False, variable = jack)
setupmenu.add_checkbutton(label = "Jill present", onvalue = True, offvalue = False, variable = jill)
menubar.add_cascade(label = "SetUp", menu = setupmenu)

# Help menu
helpmenu = Menu(menubar, tearoff=1)
helpmenu.add_command(label="Help Docs")
helpmenu.add_command(label="About", command = mAbout)
menubar.add_cascade(label="Help", menu=helpmenu)

root.config(menu=menubar)

# Radio 1
radio11 = Radiobutton(root, text="H 1", value = 1, variable = radiovar1)
radio11.grid(row=2,column=1)
radio12 = Radiobutton(root, text="H 2", value = 2, variable = radiovar1)
radio12.grid(row=2,column=2)
radio13 = Radiobutton(root, text="H 3", value = 3, variable = radiovar1)
radio13.grid(row=2,column=3)

# Radio 2
radio21 = Radiobutton(root, text="V 1", value = 1, variable = radiovar2, indicatoron=0)
radio21.grid(row=2,column=0)
radio22 = Radiobutton(root, text="V 2", value = 2, variable = radiovar2, indicatoron=0)
radio22.grid(row=3,column=0)
radio23 = Radiobutton(root, text="V 3", value = 3, variable = radiovar2, indicatoron=0)
radio23.grid(row=4,column=0)

# Spin Box
#                                              DISABLED
label3 = Label(text='SpinBox->', fg='green', bg='white')
label3.grid(row=3, column=1, sticky=E)
spinbox1 = Spinbox(root, from_=0, to=50, state=NORMAL, width=5, textvariable=spinvar)
spinbox1.grid(row=3,column=2)

# List Box
List1 = Listbox(root, selectmode=MULTIPLE, height=6)
List1.grid(row=4, column=0, rowspan=6)
List1.insert(0, 'Python')
List1.insert(1, 'C++')
List1.insert(2, 4)
List1.insert(3, 'C')
List1.insert(END, 'Octave')
# List1.delete(2)

# Slider or Scale
slider1 = Scale(root, orient=HORIZONTAL, length=300, width=10, from_=0, to=50,
    tickinterval=10, variable=slidervar)
slider1.grid(row=4, column=2, columnspan=3)

# Check Button
check1 = Checkbutton(root, text="Emer Ovr", state=ACTIVE, variable=checkbtnvar,
    offvalue=False, onvalue=True, indicatoron=0)
check1.grid(row=0, column=2)

# Busy/Proceed
busyprostr.set("Proceed")
busypro = Button(textvariable = busyprostr, command=BusyProceed, bg="Green",
                 fg="white", disabledforeground="Black")
busypro.grid(row=0, column=3)

# Canvas
theCanvas = Canvas(root, height=300, width=600, bg="white", cursor="crosshair")
theCanvas.grid(row=5,column=1,columnspan=6, sticky=SW)

theCanvas.bind("<Button-1>", CanvasMouseButton)
theCanvas.bind("<Motion>"  , CanvasXY)

theCanvas.create_line(0,0,750,450, fill="green")
theCanvas.create_oval(50,150,200,250, outline="blue")
theCanvas.create_rectangle(50,150,150,200, outline="red")
theCanvas.create_text(500, 50, text="Sample Text")
coords = 200,100,440,210
myarc = theCanvas.create_arc(coords, start=15, extent=100, fill="red")

# Key

label2.bind(sequence="<Key>", func=Key)

# Let Windows have the thread till we exit
root.mainloop()
