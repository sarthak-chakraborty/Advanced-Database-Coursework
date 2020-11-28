----------- Q1 -------------
select s._state, s.city, s.pincode, sum(p.qty)
from dsk_storedim s, dsk_purchasefact p
where s.scode = p.scode
group by s._state, s.city, s.pincode;


----------- Q2 -------------
select s.sname, i.iname, d.purchasedate, p.qty
from dsk_storedim s, dsk_itemdim i, dsk_dateofpurchasedim d, dsk_purchasefact p
where s.scode = p.scode and i.icode=p.icode and d.dcode = p.dcode;


----------- Q3 -------------
select s.city, i.category, sum(p.qty*i.price)
from dsk_storedim s, dsk_itemdim i, dsk_purchasefact p
where s.scode = p.scode and i.icode=p.icode
group by s.city, i.category;


----------- Q4 -------------
select s.city, i.subcategory, d.month, sum(p.qty*i.price)
from dsk_storedim s, dsk_itemdim i, dsk_dateofpurchasedim d, dsk_purchasefact p
where s.scode = p.scode and i.icode=p.icode and d.dcode = p.dcode
group by s.city, i.subcategory, d.month;


----------- Q5 -------------
select s._state, s.city, s.pincode, i.category, i.subcategory, d.year, d.quarter, d.month, d.dayoftheweek, sum(p.qty*i.price)
from dsk_storedim s, dsk_itemdim i, dsk_dateofpurchasedim d, dsk_purchasefact p
where s.scode = p.scode and i.icode = p.icode and d.dcode = p.dcode
group by s._state, s.city, s.pincode, i.category, i.subcategory, d.year, d.quarter, d.month, d.dayoftheweek;