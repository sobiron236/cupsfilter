
--delete from printers;
--delete from printers_mandat;
--select * from printers;
--select set_printer_id ('PDF','192.168.11.151','SYSPROG');

--select * from printers;
--select get_printer_id('HPLaserJet','192.168.11.1','SYSPROG');

--select set_reports_short_info('192.168.11.99','HPLaserJet5','SY','guest','��� ������������ �����.doc',6,'������','������ ������� ���������.��� �������� �������������');
--select * from reports


/*
select set_reports_info('192.168.11.99','HPLaserJet5','SYSPROG','guest','��� ������������ �����.doc',6,
			      '������','������ ������� ���������,��� �������� �������������',
			      'd/:/test.pdf',9,'��','��14/14',99,'��������','�����','������������','+7964xxxxxxx');

*/
--select get_doclevel_id('��');
--select * from document_level;

--select * from debug_log;

--insert into debug_log (info_str) values(E'SQLShell.pm:: When execute query SELECT check_printer_perm(\'PDF\',\'192.168.11.150\',\'SYSPROG\',\'N\'):');
select check_printer_perm('PDF','192.168.11.200','SYSPROG','��')