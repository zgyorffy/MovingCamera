a = [42,44,48,51,54,58,62,65,70,73,78,
     82,85,88,92,96,100,104,108,111,115,
     119,123,127,131,134,139,142,147,151,
     154,160,164,167,171,174]


lo = 0
hi = int(len(a) - 1)

speed = 120

while lo <= hi:
    mid = int((hi + lo) / 2)

    if speed < a[mid]:
        hi = mid - 1
    elif speed > a[mid]:
        lo = mid + 1
    else:
        speed_real = a[mid]
        print(speed_real)
        print(mid)
        break

if a[lo] - speed < speed - a[hi]:
    speed_real = a[lo]
    print(lo)
else:
    speed_real = a[hi]
    print(hi)

print(speed_real)
