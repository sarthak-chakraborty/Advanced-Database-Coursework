set @@max_heap_table_size=3221225472;
set global max_allowed_packet=1073741824;

create table dsk_storedim(
    scode int not null,
    sname varchar(30) not null,
    city varchar(30) not null,
    _state varchar(20) not null,
    pincode int not null,
    primary key (scode),
    constraint chk_city check (city in ('Mumbai','Chennai','Kolkata','Pune','Bengaluru','Guwahati','Kochi')),
    constraint chk_state check (_state in ('Kerala','Maharashtra','West Bengal','Karnataka','Tamil Nadu','Assam')),
    constraint chk_pincode check (pincode in (123456, 234567, 345678, 456789, 567890, 678901, 789012, 890123, 901234))
)ENGINE=innodb;

create table dsk_itemdim(
    icode int not null,
    iname varchar(30) not null,
    category char(5) not null,
    subcategory char(3) not null,
    price int,
    primary key (icode),
    constraint chk_cat check (category in ('GROCR', 'CONSM', 'FASHN', 'MEDCN', 'VGTBL')),
    constraint chk_sub check (subcategory in ('PRS', 'OFF', 'MIL'))
)ENGINE=innodb;

create table dsk_dateofpurchasedim(
    dcode int not null,
    purchasedate date not null,
    dayoftheweek char(3),
    month char(3),
    quarter char(2),
    year int,
    primary key (dcode),
    constraint chk_day check (dayoftheweek in ('MON', 'TUE', 'WED', 'THR', 'FRI', 'SAT', 'SUN')),
    constraint chk_month check (month in ('JAN', 'FEB', 'MAR', 'APR', 'MAY', 'JUN', 'JUL', 'AUG', 'SEP', 'OCT', 'NOV', 'DEC')),
    constraint chk_quat check (quarter in ('Q1', 'Q2', 'Q3', 'Q4')),
    constraint chk_year check (year in (2015, 2016, 2017, 2018, 2019, 2020))
)ENGINE=innodb;

create table dsk_purchasefact(
    scode int not null,
    icode int not null,
    dcode int not null,
    qty int not null,
    primary key (scode, icode, dcode),
    foreign key (scode) references dsk_storedim(scode),
    foreign key (icode) references dsk_itemdim(icode),
    foreign key (dcode) references dsk_dateofpurchasedim(dcode)
)ENGINE=innodb;
