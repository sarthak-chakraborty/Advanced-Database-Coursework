import os
import string 
import random 

dsk_tab = 'insert_storedim_disk.sql'
inm_tab = 'insert_storedim_inm.sql'
clm_tab = 'insert_storedim_mariadb.sql'
mon_tab = 'insert_storedim_monetdb.sql'

num_rows = 1000
sname_len = 10

city_state_list = [('Mumbai' , 'Maharashtra'),
              ('Chennai' , 'Tamil Nadu'),
              ('Kolkata' , 'West Bengal'),
              ('Pune' , 'Maharashtra'),
              ('Bengaluru' , 'Karnataka'),
              ('Guwahati' , 'Assam'),
              ('Kochi' , 'Kerala')]
pincode_list = [123456, 234567, 345678, 456789, 567890, 678901, 789012, 890123, 901234]

f_dsk = open(dsk_tab, 'w')
f_inm = open(inm_tab, 'w')
f_clm = open(clm_tab, 'w')
f_mon = open(mon_tab, 'w')


STR_dsk = 'insert into dsk_storedim values '
STR_inm = 'insert into inm_storedim values '
STR_clm = 'insert into clm_storedim values '
STR_mon = 'insert into storedim values '
f_dsk.write(STR_dsk)
f_inm.write(STR_inm)
f_clm.write(STR_clm)
f_mon.write(STR_mon)


for i in range(num_rows):
    scode = i
    sname = ''.join(random.choices(string.ascii_uppercase + string.digits, k = sname_len))
    city_state = random.choice(city_state_list)
    city = city_state[0]
    state = city_state[1]
    pincode = random.choice(pincode_list)

    STR_dsk = '({},\'{}\',\'{}\',\'{}\',{})'.format(scode,sname,city,state,pincode)
    STR_inm = '({},\'{}\',\'{}\',\'{}\',{})'.format(scode,sname,city,state,pincode)
    STR_clm = '({},\'{}\',\'{}\',\'{}\',{})'.format(scode,sname,city,state,pincode)
    STR_mon = '({},\'{}\',\'{}\',\'{}\',{})'.format(scode,sname,city,state,pincode)
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