insert into document_level (marker,sec_level,ds) values ('��','0','�� ��������');
insert into document_level (marker,sec_level,ds) values ('�','1','��������');
insert into document_level (marker,sec_level,ds) values ('��','2','���������� ��������');
insert into document_level (marker,sec_level,ds) values ('��','3','������ ��������');

insert into rel_mandat2doc_level(mandat_id,doclevel_id) values(1,1);
insert into rel_mandat2doc_level(mandat_id,doclevel_id) values(1,2);
insert into rel_mandat2doc_level(mandat_id,doclevel_id) values(1,3);
insert into rel_mandat2doc_level(mandat_id,doclevel_id) values(1,4);
insert into rel_mandat2doc_level(mandat_id,doclevel_id) values(2,1);

select * from document_level