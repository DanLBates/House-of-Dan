'''
I want to see this work and not just be theoretical.
'''

import asyncio

async def say (what, when):
    await asyncio.sleep(when)
    print (what)

print("Can only do one of these all or any")
while(True):
    usersays = input("all/any? ")
    if usersays == "all":
        all = True
        break
    if usersays == "any":
        all = False
        break

loop = asyncio.get_event_loop()
t1 = loop.create_task(say("hello", 0.5))
t2 = loop.create_task(say("world", 1))
t3 = loop.create_task(say("greetings",1.5))
t4 = loop.create_task(say("earth", 2.0))

try:
    if all:
        loop.run_until_complete(asyncio.gather(t1,t2,t3,t4))
    else:
        loop.run_until_complete(asyncio.wait([t1,t2,t3,t4], return_when=asyncio.FIRST_COMPLETED))
finally:
    loop.close()

del loop
