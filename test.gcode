G01 X1.353 Y-14.5 Z8 F156
G01 X-1.2 Y-14.5 Z12 F400
G01 X-132.23 Y1.223 Z235.645 F1.01 
G00 X15 Y1
G02 X12 Y12.399 I12 J0 F1
G01 Y1 Z0.01 X0.5 F400
G03 X5 Y71.5 I12     ;J1
G18
M25 ; Test comment, seems to work
M12 ; This is a test comment, should not be outputted, X12, also should record this value
M13
M13
M12
G01 X1
M05
M25
G28 X13 Y12
M16

