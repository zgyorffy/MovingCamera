import time

ms = 0
timestamp = time.time()
while(ms < 5.0):
    ms = round((time.time() - timestamp), 1)
    print(ms)

