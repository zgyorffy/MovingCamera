import time

def ms_now(time_st):
    return (round((time.time() - time_st), 1))

def speed_search(speed_list, speed):
    lo = 0
    hi = int(len(speed_list) - 1)

    if speed < speed_list[0]:
        return (speed_list[0], 0)

    if speed > speed_list[-1]:
        return (speed_list[-1], hi)

    while lo <= hi:
        mid = int((hi + lo) / 2)

        if speed < a[mid]:
            hi = mid - 1
        elif speed > a[mid]:
            lo = mid + 1
        else:
            return(speed_list[mid], mid)

    if speed_list[lo] - speed < speed - speed_list[hi]:
        return (speed_list[lo], lo)
    else:
        return (speed_list[hi], hi)


timestamp = time.time()

a = [42,44,48,51,54,58,62,65,70,73,78,
     82,85,88,92,96,100,104,108,111,115,
     119,123,127,131,134,139,142,147,151,
     154,160,164,167,171,174]

acc = 0.2

sp = 255
sp,steps = speed_search(a,sp)

dMax = 200

start_speed = 3
max_speed = steps * 0.5 + start_speed
average_speed = (start_speed + max_speed) / 2
#time to max speed
ta = steps * 0.2
dA = round(average_speed * ta, 1)
dB = round(average_speed * ta, 1)
dC = dMax - (dA + dB)
tc = dC / max_speed
tt = 2 * ta + tc

print("vm",max_speed)
print("va:",average_speed)
print("ta:",ta)
print("tc:",tc)
print("tt:",tt)
print("dA:",dA)
print("dC:",dC)

ms = ms_now(timestamp)
print("\n")
print(sp,";",steps,";",ms)

