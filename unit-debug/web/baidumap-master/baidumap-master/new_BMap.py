# -*- coding: utf-8 -*-
latis = []
longs = []
location = []
filename = "cmxgpsinfo.txt"
f = open(filename, "r")
f1 = open('a.txt', "w+")

for line in f.readlines():
    arrline = line.split("  ")
    latis.append(arrline[0])
    longs.append(arrline[1])
    location.append([float(arrline[0]), float(arrline[1])])

print(location)


for i in location:
    f1.write("new BMap.Point({},{}),".format(str(i[0]), str(i[1])))
    f1.write('\n')


f.close()
f1.close()


