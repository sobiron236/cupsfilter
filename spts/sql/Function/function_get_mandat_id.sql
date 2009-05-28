
DROP FUNCTION get_mandat_id(character varying);

CREATE OR REPLACE FUNCTION get_mandat_id(character varying)
  RETURNS integer AS
$BODY$
    DECLARE
        mandat_id INTEGER;
        notfound_id INTEGER;
    BEGIN
    /*
	�������� id ������� ���� ������ �� ����������,
		�� ���������� ������������� ������� "�� ���������"
			���� ��� ��� �
				�������� � �������, �������� ��������� � ������� ���� 
		�������  id ���������� �������    
    */
	notfound_id := (select id from printers_mandat where printers_mandat.mandat='NOT FOUND'); 
	IF notfound_id ISNULL THEN
		-- ������� � ������� ������
		INSERT INTO printers_mandat (mandat) VALUES ('NOT FOUND');
		notfound_id := (select id from printers_mandat where printers_mandat.mandat='NOT FOUND'); 
	END IF;
	mandat_id := (select id from printers_mandat where printers_mandat.mandat=$1); 
        IF mandat_id ISNULL THEN
		-- ������ �������� �� ��������� 
		return notfound_id;	
        END IF;
        RETURN mandat_id;
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION get_mandat_id(character varying) OWNER TO postgres;

