import os
import string 
import random 

dsk_tab = 'insert_dsk_fact.sql'
inm_tab = 'insert_inm_fact.sql'
clm_tab = 'insert_clm_fact.sql'

num_rows = 10000000
max_range = 999

f_dsk = open(dsk_tab, 'r')
f_inm = open(inm_tab, 'r')
f_clm = open(clm_tab, 'r')

f_dsk_100 = open('insert_dsk_fact_100.sql','w')
f_inm_100 = open('insert_inm_fact_100.sql','w')
f_clm_100 = open('insert_clm_fact_100.sql','w')

f_dsk_1000 = open('insert_dsk_fact_1000.sql','w')
f_inm_1000 = open('insert_inm_fact_1000.sql','w')
f_clm_1000 = open('insert_clm_fact_1000.sql','w')

f_dsk_100000 = open('insert_dsk_fact_100000.sql','w')
f_inm_100000 = open('insert_inm_fact_100000.sql','w')
f_clm_100000 = open('insert_clm_fact_100000.sql','w')

f_dsk_1000000 = open('insert_dsk_fact_1000000.sql','w')
f_inm_1000000 = open('insert_inm_fact_1000000.sql','w')
f_clm_1000000 = open('insert_clm_fact_1000000.sql','w')

f_dsk_10000000 = open('insert_dsk_fact_10000000.sql','w')
f_inm_10000000 = open('insert_inm_fact_10000000.sql','w')
f_clm_10000000 = open('insert_clm_fact_10000000.sql','w')


i=0
for line in f_dsk:
	print(i,end='\r')

	if i < 100:
		if i == 99:
			line = line[:-2] + ';\n'
		f_dsk_100.write(line)
	elif i >= 100 and i < 1000:
		if i == 999:
			line = line[:-2] + ';\n'
		elif i == 100:
			line = 'insert into dsk_purchasefact values ' + line
		f_dsk_1000.write(line)
	elif i >= 1000 and i < 100000:
		if i == 99999:
			line = line[:-2] + ';\n'
		elif i == 1000:
			line = 'insert into dsk_purchasefact values ' + line
		f_dsk_100000.write(line)
	elif i >= 100000 and i < 1000000:
		if i == 999999:
			line = line[:-2] + ';\n'
		elif i == 100000:
			line = 'insert into dsk_purchasefact values ' + line
		f_dsk_1000000.write(line)
	else:
		if i == 9999999:
			line = line[:-2] + ';\n'
		elif i == 1000000:
			line = 'insert into dsk_purchasefact values ' + line
		f_dsk_10000000.write(line)

	i+=1

print('dsk_done')

i=0
for line in f_inm:
	print(i,end='\r')

	if i < 100:
		if i == 99:
			line = line[:-2] + ';\n'
		f_inm_100.write(line)
	elif i >= 100 and i < 1000:
		if i == 999:
			line = line[:-2] + ';\n'
		elif i == 100:
			line = 'insert into inm_purchasefact values ' + line
		f_inm_1000.write(line)
	elif i >= 1000 and i < 100000:
		if i == 99999:
			line = line[:-2] + ';\n'
		elif i == 1000:
			line = 'insert into inm_purchasefact values ' + line
		f_inm_100000.write(line)
	elif i >= 100000 and i < 1000000:
		if i == 999999:
			line = line[:-2] + ';\n'
		elif i == 100000:
			line = 'insert into inm_purchasefact values ' + line
		f_inm_1000000.write(line)
	else:
		if i == 9999999:
			line = line[:-2] + ';\n'
		elif i == 1000000:
			line = 'insert into inm_purchasefact values ' + line
		f_inm_10000000.write(line)

	i+=1

print('inm done')



i=0
for line in f_clm:
	print(i,end='\r')

	if i < 100:
		if i == 99:
			line = line[:-2] + ';\n'
		f_clm_100.write(line)
	elif i >= 100 and i < 1000:
		if i == 999:
			line = line[:-2] + ';\n'
		elif i == 100:
			line = 'insert into clm_purchasefact values ' + line
		f_clm_1000.write(line)
	elif i >= 1000 and i < 100000:
		if i == 99999:
			line = line[:-2] + ';\n'
		elif i == 1000:
			line = 'insert into clm_purchasefact values ' + line
		f_clm_100000.write(line)
	elif i >= 100000 and i < 1000000:
		if i == 999999:
			line = line[:-2] + ';\n'
		elif i == 100000:
			line = 'insert into clm_purchasefact values ' + line
		f_clm_1000000.write(line)
	else:
		if i == 9999999:
			line = line[:-2] + ';\n'
		elif i == 1000000:
			line = 'insert into clm_purchasefact values ' + line
		f_clm_10000000.write(line)

	i+=1

print('clm done')



f_dsk.close()
f_inm.close()
f_clm.close()
f_dsk_100.close()
f_inm_100.close()
f_clm_100.close()
f_dsk_1000.close()
f_inm_1000.close()
f_clm_1000.close()
f_dsk_100000.close()
f_inm_100000.close()
f_clm_100000.close()
f_dsk_1000000.close()
f_inm_1000000.close()
f_clm_1000000.close()
f_dsk_10000000.close()
f_inm_10000000.close()
f_clm_10000000.close()
