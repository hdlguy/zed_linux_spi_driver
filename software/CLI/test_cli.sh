./cli << !
#use less line 1
w 0 aaaaaaaa
w 4 bbbbbbbb
w 8 cccccccc
w c dddddddd
w 10 eeeeeeee
r 8
r 10
#use less line 2
r 3c00000
q
!
