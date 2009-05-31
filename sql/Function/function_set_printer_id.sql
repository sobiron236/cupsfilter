--DROP FUNCTION set_printer_id(character varying,inet,character varying);

CREATE OR REPLACE FUNCTION set_printer_id(
				character varying,-- ��� �������� 
				inet,		-- Ip �����
				character varying -- ������ 
				)
  RETURNS integer AS
$BODY$
    /*
	�������� ID ������� �������� (������� ������ ��������� ��������, ���� ������ �� ���������� �� �������� ID ������� UNDEF_MANDAT)

	�������� id �������� (� ����������� IP,��� ��������,������)���� ������ �� ����������,
		�� �������� ������ � ������� ��������� ������� ID ������, �������� ��������� � ������� ���� 

	��������� ��� ����� ��� ������� ����� ���������� �� �����������, � ����� ���������� �� � ����
    */
    DECLARE 
	m_id INTEGER; -- ������ ID
	p_id INTEGER; -- ������� ID
	-- ���������� ����������� ��� ���� ���������� �������,
	p_name ALIAS for $1;
	ip ALIAS for $2;
	mandat ALIAS for $3;
    BEGIN
	m_id := set_mandat_id(mandat);
	p_id :=(select id from printers where printer_name=p_name and ip_adress=ip and mandat_id=m_id); 
	IF p_id IS NULL THEN	
		INSERT INTO debug_log (info_str) VALUES ('������� '||p_name||' c ����������� IP '|| host(ip) ||' �� ������������ ������, ��������');
		INSERT INTO printers (printer_name,ip_adress,mandat_id) VALUES (p_name,ip,m_id);
		RETURN (SELECT id FROM printers WHERE printer_name=p_name and ip_adress=ip and mandat_id=m_id);
	END IF;
	RETURN p_id;
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION set_printer_id(character varying,inet,character varying) OWNER TO postgres;

