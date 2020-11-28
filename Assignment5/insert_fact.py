import os
import string 
import random 

dsk_tab = 'insert_fact_disk.sql'
inm_tab = 'insert_fact_inm.sql'
clm_tab = 'insert_fact_mariadb.sql'
mon_tab = 'insert_fact_monetdb.sql'

num_rows = 10000000
max_range = 999

f_dsk = open(dsk_tab, 'w')
f_inm = open(inm_tab, 'w')
f_clm = open(clm_tab, 'w')
f_mon = open(mon_tab, 'w')


STR_dsk = 'insert into dsk_purchasefact values '
STR_inm = 'insert into inm_purchasefact values '
STR_clm = 'insert into clm_purchasefact values '
STR_mon = 'insert into purchasefact values '
f_dsk.write(STR_dsk)
f_inm.write(STR_inm)
f_clm.write(STR_clm)
f_mon.write(STR_mon)


primary_key = {}

for i in range(num_rows):
    print(i,end='\r')
    while(1):
	    scode = random.randint(0, max_range)
	    icode = random.randint(0, max_range)
	    dcode = random.randint(0, max_range)

	    if (scode,icode,dcode) not in primary_key:
	    	primary_key[(scode,icode,dcode)] = 1
	    	break

    qty = random.randint(10, 1000)

    STR_dsk = '({},{},{},{})'.format(scode,icode,dcode,qty)
    STR_inm = '({},{},{},{})'.format(scode,icode,dcode,qty)
    STR_clm = '({},{},{},{})'.format(scode,icode,dcode,qty)
    STR_mon = '({},{},{},{})'.format(scode,icode,dcode,qty)
    if i == num_rows-1:
    	STR_dsk += ';\n'
    	STR_inm += ';\n'
    	STR_clm += ';\n'
    	STR_mon += ';\n'
    else:
    	STR_dsk += ',\n'
    	STR_inm += ',\n'
    	STR_clm += ',\n'
    	STR_mon += ',\n'

    f_dsk.write(STR_dsk)
    f_inm.write(STR_inm)
    f_clm.write(STR_clm)
    f_mon.write(STR_mon)

f_dsk.close()
f_inm.close()
f_clm.close()
f_mon.close()