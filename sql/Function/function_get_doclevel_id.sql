--DROP FUNCTION get_firstpages_id(character varying);

CREATE OR REPLACE FUNCTION get_doclevel_id(character varying)
  RETURNS integer AS
$BODY$
    DECLARE 
	dl_id INTEGER; -- ID ������ ���������� �������(����) ��������� ���� ������ ��� �� ������� ����������� ���� ��������� ��
	undef_id INTEGER;
    BEGIN
	dl_id :=(select id from document_level where marker=$1); 
	IF dl_id IS NULL THEN	
		INSERT INTO debug_log(info_str) values ('�� ������ ���� ��������� ' ||$1);
		undef_id := (select id from document_level where marker='��'); 
		IF undef_id ISNULL THEN
			-- ������� � ������� ������
			INSERT INTO debug_log(info_str) values ('�������������� ���������� ����� ��������� ��');
			INSERT INTO document_level (marker) VALUES ('��');
			undef_id := (select id from document_level where marker='��'); 
		END IF;
		RETURN undef_id; 
	END IF;
	RETURN dl_id;
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION get_doclevel_id(character varying) OWNER TO postgres;


	
