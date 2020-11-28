import os
import string 
import random 

dsk_tab = 'insert_dateofpurchasedim_disk.sql'
inm_tab = 'insert_dateofpurchasedim_inm.sql'
clm_tab = 'insert_dateofpurchasedim_mariadb.sql'
mon_tab = 'insert_dateofpurchasedim_monetdb.sql'

num_rows = 1000

month_list = {'01' : ('JAN',31,'Q4'),
              '02' : ('FEB',28,'Q4'),
              '03' : ('MAR',31,'Q4'),
              '04' : ('APR',30,'Q1'),
              '05' : ('MAY',31,'Q1'),
              '06' : ('JUN',30,'Q1'),
              '07' : ('JUL',31,'Q2'),
              '08' : ('AUG',31,'Q2'),
              '09' : ('SEP',30,'Q2'),
              '10' : ('OCT',31,'Q3'),
              '11' : ('NOV',30,'Q3'),
              '12' : ('DEC',31,'Q3')}
day_list = ['MON', 'TUE', 'WED', 'THR', 'FRI', 'SAT', 'SUN']
year_list = [2015, 2016, 2017, 2018, 2019, 2020]

f_dsk = open(dsk_tab, 'w')
f_inm = open(inm_tab, 'w')
f_clm = open(clm_tab, 'w')
f_mon = open(mon_tab, 'w')


STR_dsk = 'insert into dsk_dateofpurchasedim values '
STR_inm = 'insert into inm_dateofpurchasedim values '
STR_clm = 'insert into clm_dateofpurchasedim values '
STR_mon = 'insert into dateofpurchasedim values '
f_dsk.write(STR_dsk)
f_inm.write(STR_inm)
f_clm.write(STR_clm)
f_mon.write(STR_mon)


for i in range(num_rows):
    dcode = i
    dayoftheweek = random.choice(day_list)
    month_date = random.choice(list(month_list.keys()))
    year = random.choice(year_list)
    month = month_list[month_date][0]
    tot_days = month_list[month_date][1]+1 if year%4==0 and month=='02' else month_list[month_date][1]
    quarter = month_list[month_date][2]
    purchasedate = '{}-{}-{}'.format( str(year), month_date, str(random.randint(1,tot_days)) )

    STR_dsk = '({},\'{}\',\'{}\',\'{}\',\'{}\',{})'.format(dcode,purchasedate,dayoftheweek,month,quarter,year)
    STR_inm = '({},\'{}\',\'{}\',\'{}\',\'{}\',{})'.format(dcode,purchasedate,dayoftheweek,month,quarter,year)
    STR_clm = '({},\'{}\',\'{}\',\'{}\',\'{}\',{})'.format(dcode,purchasedate,dayoftheweek,month,quarter,year)
    STR_mon = '({},\'{}\',\'{}\',\'{}\',\'{}\',{})'.format(dcode,purchasedate,dayoftheweek,month,quarter,year)
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