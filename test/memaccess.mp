#
# test case: memory access
#
# output:
# 0x00000000
# [endianness of the system]
#

map 0x0000 0x1000 "/dev/zero"

print hex:32 peek:32(0)

poke:32 0 0x12345678

a := peek:16(0)
b := peek:16(2)

if a == 0x1234 && b == 0x5678 then print "big endian"
else if a == 0x5678 && b == 0x1234 then print "little endian"
else print "unknown endian"
