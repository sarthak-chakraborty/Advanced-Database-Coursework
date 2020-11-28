create table storedim(
    scode int not null primary key,
    sname varchar(30) not null,
    city varchar(30) not null,
    _state varchar(20) not null,
    pincode int not null
);

create table itemdim(
    icode int not null primary key,
    iname varchar(30) not null,
    category char(5) not null,
    subcategory char(3) not null,
    price int
);

create table dateofpurchasedim(
    dcode int not null primary key,
    purchasedate date not null,
    dayoftheweek char(3),
    _month char(3),
    quarter char(2),
    _year int
);

create table purchasefact(
    scode int not null,
    icode int not null,
    dcode int not null,
    qty int not null,
    primary key (scode, icode, dcode),
    foreign key (scode) references storedim(scode),
    foreign key (icode) references itemdim(icode),
    foreign key (dcode) references dateofpurchasedim(dcode)
);
