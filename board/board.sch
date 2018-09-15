v 20130925 2
C 40000 40000 0 0 0 title-B.sym
C 53300 45800 1 0 0 led-3.sym
{
T 54250 46450 5 10 0 0 0 0 1
device=LED
T 53750 46350 5 10 1 1 0 0 1
refdes=D1
T 53300 45800 5 10 1 0 0 0 1
footprint=LED 100
}
C 54800 46600 1 90 0 resistor-1.sym
{
T 54400 46900 5 10 0 0 90 0 1
device=RESISTOR
T 54500 46800 5 10 1 1 90 0 1
refdes=R1
T 54800 46600 5 10 1 1 0 0 1
value=2.7k
T 54800 46600 5 10 1 0 0 0 1
footprint=AXIAL_LAY 300
}
N 54200 46000 54700 46000 4
N 54700 46000 54700 46600 4
C 42300 43000 1 0 0 connector14-2.sym
{
T 43000 49100 5 10 1 1 0 6 1
refdes=CONN1
T 42600 49050 5 10 0 0 0 0 1
device=CONNECTOR_14
T 42600 49250 5 10 0 0 0 0 1
footprint=JUMPER14
}
C 46300 49000 1 180 0 connector14-2.sym
{
T 45600 42800 5 10 1 1 180 6 1
refdes=CONN2
T 46000 42950 5 10 0 0 180 0 1
device=CONNECTOR_14
T 46000 42750 5 10 0 0 180 0 1
footprint=JUMPER14
}
T 46200 47500 9 10 1 0 0 0 1
13
T 46200 47100 9 10 1 0 0 0 1
12
T 46200 46700 9 10 1 0 0 0 1
11
T 46200 46300 9 10 1 0 0 0 1
10
T 46300 45900 9 10 1 0 0 0 1
9
T 46300 45300 9 10 1 0 0 0 2
7

T 46300 45100 9 10 1 0 0 0 1
5!
T 46300 44700 9 10 1 0 0 0 1
3/SCL
T 46300 44300 9 10 1 0 0 0 1
2/SDA
T 46300 43900 9 10 1 0 0 0 1
1/TX
T 46300 43500 9 10 1 0 0 0 1
0/RX
T 42300 43500 9 10 1 0 0 0 1
G
T 42200 43900 9 10 1 0 0 0 1
MI
T 42200 44300 9 10 1 0 0 0 1
MO
T 42200 44700 9 10 1 0 0 0 1
SCK
T 42200 48800 9 10 1 0 0 0 1
RST
T 42200 48300 9 10 1 0 0 0 1
3V
T 46200 48800 9 10 1 0 0 0 1
BAT
T 46300 48300 9 10 1 0 0 0 1
G
T 46300 47900 9 10 1 0 0 0 1
USB
N 41600 48200 42300 48200 4
N 42300 43800 41700 43800 4
N 41700 41100 41700 43800 4
N 41700 41100 49100 41100 4
N 49100 41100 49200 46300 4
N 49200 46300 50700 46300 4
N 50700 45800 49500 45800 4
N 49500 45800 49400 40900 4
N 49400 40900 41100 40900 4
N 41100 40900 41100 44600 4
N 41100 44600 42300 44600 4
N 42300 44200 42100 44200 4
N 42100 44200 42100 41500 4
N 49700 44300 50700 44800 4
N 49700 44300 49700 41500 4
N 49700 41500 42100 41500 4
N 50700 44300 47300 44300 4
N 46300 43800 47200 43800 4
N 46300 43400 47900 43400 4
N 47900 43400 48000 45300 4
N 48000 45300 50700 45300 4
N 47300 44300 47200 43800 4
C 47200 47900 1 0 0 gnd-1.sym
C 42200 43100 1 0 0 gnd-1.sym
N 46300 48200 47300 48200 4
C 54500 47500 1 0 0 generic-power.sym
{
T 54700 47750 5 10 1 1 0 3 1
net=Vcc:1
}
C 41400 48200 1 0 0 generic-power.sym
{
T 41600 48450 5 10 1 1 0 3 1
net=Vcc:1
}
C 47600 47200 1 0 0 switch-pushbutton-no-1.sym
{
T 48000 47500 5 10 1 1 0 0 1
refdes=S1
T 48000 47800 5 10 0 0 0 0 1
device=SWITCH_PUSHBUTTON_NO
T 47600 47200 5 10 1 0 0 0 1
footprint=CherryMX_PCBmount_NoDiodeNoLED
}
C 47600 46500 1 0 0 switch-pushbutton-no-1.sym
{
T 48000 46800 5 10 1 1 0 0 1
refdes=S2
T 48000 47100 5 10 0 0 0 0 1
device=SWITCH_PUSHBUTTON_NO
T 47600 46500 5 10 1 0 0 0 1
footprint=CherryMX_PCBmount_NoDiodeNoLED
}
C 47600 45800 1 0 0 switch-pushbutton-no-1.sym
{
T 48000 46100 5 10 1 1 0 0 1
refdes=S3
T 48000 46400 5 10 0 0 0 0 1
device=SWITCH_PUSHBUTTON_NO
T 47600 45800 5 10 1 0 0 0 1
footprint=CherryMX_PCBmount_NoDiodeNoLED
}
C 47600 45100 1 0 0 switch-pushbutton-no-1.sym
{
T 48000 45400 5 10 1 1 0 0 1
refdes=S4
T 48000 45700 5 10 0 0 0 0 1
device=SWITCH_PUSHBUTTON_NO
T 47600 45100 5 10 1 0 0 0 1
footprint=CherryMX_PCBmount_NoDiodeNoLED
}
N 46300 45400 47600 45400 4
N 47600 45400 47600 45100 4
N 46300 45800 47600 45800 4
N 46300 46200 47600 46200 4
N 47600 46200 47600 46500 4
N 46300 46600 47600 46600 4
N 47600 46600 47600 47200 4
C 48500 46900 1 0 0 gnd-1.sym
C 48500 46200 1 0 0 gnd-1.sym
C 48500 45500 1 0 0 gnd-1.sym
C 48500 44800 1 0 0 gnd-1.sym
C 50400 44100 1 0 0 adns-5030-2.sym
{
T 52100 46100 5 10 1 1 0 6 1
refdes=U1
T 51900 47250 5 10 0 0 0 0 1
device=ADNS-5030
}
