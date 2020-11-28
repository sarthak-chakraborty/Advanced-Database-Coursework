----------- Q1 -------------
select s._state, s.city, s.pincode, sum(p.qty)
from storedim s, purchasefact p
where s.scode = p.scode
group by s._state, s.city, s.pincode;


----------- Q2 -------------
select s.sname, i.iname, d.purchasedate, p.qty
from storedim s, itemdim i, dateofpurchasedim d, purchasefact p
where s.scode = p.scode and i.icode=p.icode and d.dcode = p.dcode;


----------- Q3 -------------
select s.city, i.category, sum(p.qty*i.price)
from storedim s, itemdim i, purchasefact p
where s.scode = p.scode and i.icode=p.icode
group by s.city, i.category;


----------- Q4 -------------
select s.city, i.subcategory, d._month, sum(p.qty*i.price)
from storedim s, itemdim i, dateofpurchasedim d, purchasefact p
where s.scode = p.scode and i.icode=p.icode and d.dcode = p.dcode
group by s.city, i.subcategory, d._month;


----------- Q5 -------------
select s._state, s.city, s.pincode, i.category, i.subcategory, d._year, d.quarter, d._month, d.dayoftheweek, sum(p.qty*i.price)
from storedim s, itemdim i, dateofpurchasedim d, purchasefact p
where s.scode = p.scode and i.icode = p.icode and d.dcode = p.dcode
group by s._state, s.city, s.pincode, i.category, i.subcategory, d._year, d.quarter, d._month, d.dayoftheweek;