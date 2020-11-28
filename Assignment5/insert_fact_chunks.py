import os
import string 
import random 

inm_tab = 'insert_dsk_fact_10000000.sql'

num_rows = 1000000

f_inm = open(inm_tab, 'r')

created = set()

i=0
for line in f_inm:
	print(i,end='\r')

	num = int(i / num_rows)

	if num not in created:
		if num != 0:
			f.close()

		f = open('insert_dsk_fact_10000000_{}.sql'.format(num) , 'w')
		line = line if i == 0 else 'insert into dsk_purchasefact values ' + line

	created.add(num)

	if (i%num_rows) == num_rows-1:
		line = line[:-2] + ';\n'

	f.write(line)
	i+=1


f_inm.close()
