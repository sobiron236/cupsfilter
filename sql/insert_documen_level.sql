/*
DROP TABLE document_level;

CREATE TABLE document_level
(
  id serial NOT NULL,
  marker varchar(3) NOT NULL,
  sec_level integer NOT NULL,
  ds character varying(25) NOT NULL,
  PRIMARY KEY (id)
);

ALTER TABLE document_level OWNER TO postgres;
COMMENT ON TABLE document_level IS '������� ����������� ������ ����������� ����������:
�� ��������
��������
���������� ��������
������ ��������';


insert into document_level (marker,sec_level,ds) values ('��','0','�� ��������');
insert into document_level (marker,sec_level,ds) values ('�','1','��������');
insert into document_level (marker,sec_level,ds) values ('��','2','���������� ��������');
insert into document_level (marker,sec_level,ds) values ('��','3','������ ��������');

insert into rel_mandat2doc_level(mandat_id,doclevel_id) values(1,1);
insert into rel_mandat2doc_level(mandat_id,doclevel_id) values(1,2);
insert into rel_mandat2doc_level(mandat_id,doclevel_id) values(1,3);
insert into rel_mandat2doc_level(mandat_id,doclevel_id) values(1,4);
insert into rel_mandat2doc_level(mandat_id,doclevel_id) values(2,1);
*/

select * from document_level