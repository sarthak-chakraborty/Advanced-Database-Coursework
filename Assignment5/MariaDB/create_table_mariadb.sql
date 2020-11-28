create table clm_storedim(
    scode int not null,
    sname varchar(30) not null,
    city varchar(30) not null,
    _state varchar(20) not null,
    pincode int not null
) ENGINE=columnstore;

create table clm_itemdim(
    icode int not null,
    iname varchar(30) not null,
    category char(5) not null,
    subcategory char(3) not null,
    price int
) ENGINE=columnstore;

create table clm_dateofpurchasedim(
    dcode int not null,
    purchasedate date not null,
    dayoftheweek char(3),
    month char(3),
    quarter char(2),
    year int
) ENGINE=columnstore;

create table clm_purchasefact(
    scode int not null,
    icode int not null,
    dcode int not null,
    qty int not null
) ENGINE=columnstore;