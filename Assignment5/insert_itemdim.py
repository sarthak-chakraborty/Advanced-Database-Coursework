import os
import string 
import random 

dsk_tab = 'insert_itemdim_disk.sql'
inm_tab = 'insert_itemdim_inm.sql'
clm_tab = 'insert_itemdim_mariadb.sql'
mon_tab = 'insert_itemdim_monetdb.sql'

num_rows = 1000
iname_len = 15

category_list = ['GROCR', 'CONSM', 'FASHN', 'MEDCN', 'VGTBL']
subcategory_list = ['PRS', 'OFF', 'MIL']

f_dsk = open(dsk_tab, 'w')
f_inm = open(inm_tab, 'w')
f_clm = open(clm_tab, 'w')
f_mon = open(mon_tab, 'w')


STR_dsk = 'insert into dsk_itemdim values '
STR_inm = 'insert into inm_itemdim values '
STR_clm = 'insert into clm_itemdim values '
STR_mon = 'insert into itemdim values '
f_dsk.write(STR_dsk)
f_inm.write(STR_inm)
f_clm.write(STR_clm)
f_mon.write(STR_mon)


for i in range(num_rows):
    icode = i
    iname = ''.join(random.choices(string.ascii_lowercase + string.digits, k = iname_len))
    category = random.choice(category_list)
    subcategory = random.choice(subcategory_list)
    price = random.randint(10, 1000)

    STR_dsk = '({},\'{}\',\'{}\',\'{}\',{})'.format(icode,iname,category,subcategory,price)
    STR_inm = '({},\'{}\',\'{}\',\'{}\',{})'.format(icode,iname,category,subcategory,price)
    STR_clm = '({},\'{}\',\'{}\',\'{}\',{})'.format(icode,iname,category,subcategory,price)
    STR_mon = '({},\'{}\',\'{}\',\'{}\',{})'.format(icode,iname,category,subcategory,price)
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