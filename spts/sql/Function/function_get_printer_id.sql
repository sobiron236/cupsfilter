--DROP FUNCTION get_printer_id(character varying,inet,character varying);

CREATE OR REPLACE FUNCTION get_printer_id(
				character varying,-- ��� �������� 
				inet,		-- Ip �����
				character varying -- ������ 
				)
  RETURNS integer AS
$BODY$
    /*
	�������� ID ������� �������� (������� ������ ��������� ��������, ���� ������ �� ���������� �� �������� ID ������� PUBLIC, 
	�� ���� �� ���� �������� ����� ������ ����������� ���������)

	�������� id �������� (� ����������� IP,��� ��������,������)���� ������ �� ����������,
		�� �������� ������ � ������� ��������� ������� ID ������, �������� ��������� � ������� ���� 

	��������� ��� ����� ��� ������� ����� ���������� �� �����������, � ����� ���������� �� � ����
    */
    DECLARE 
	m_id INTEGER; -- ������ ID
	p_id INTEGER; -- ������� ID
    BEGIN
	m_id := get_mandat_id($3);
	p_id :=(select id from printers where printer_name=$1 and ip_adress=$2 and mandat_id=m_id); 
	IF p_id IS NULL THEN	
		INSERT INTO debug_log (info_str) VALUES ('������� '.$1'��������');
		INSERT INTO printers (printer_name,ip_adress,mandat_id) VALUES ($1,$3,m_id);
		RETURN (SELECT id FROM printers WHERE printer_name=$1 and ip_adress=$2 and mandat_id=m_id);
	END IF;
	RETURN p_id;
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION get_printer_id(character varying,inet,character varying) OWNER TO postgres;


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
