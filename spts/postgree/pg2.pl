#!/usr/bin/perl
#������� fetchrow_array ������ ��� ��� �������, ����� ����� ������� ���������� �������, 
#� ������� ���������� ��������� (���������� ����� � �� ��������). � ������ ������� ����� ������������ ������ �������. 
#��������, ��� �� ����� �������� �� ������� pg_tables �� ��� ����, � ������ ��� tablename � tableowner. 
#� �������� ��� ����� ������� ������ ��������� ���������� ������, �� � ���� ������, �������, 
#����� ������� ��������������� �������� fetchrow_hashref. �������� ������:

use DBI;

$dbh = DBI->connect("dbi:Pg:dbname=template1","postgres","",{PrintError => 0});

if ($DBI::err != 0) {
	print $DBI::errstr . "\n";
	exit($DBI::err);
}

$query = "SELECT * FROM pg_tables";
$sth = $dbh->prepare($query);
$rv = $sth->execute();
if (!defined $rv) {
	print "��� ���������� ������� '$query' �������� ������: " . $dbh->errstr . "\n";
	exit 1;
}

while ($ref = $sth->fetchrow_hashref()) {
	($tablename, $tableowner) = ($ref->{'tablename'}, $ref->{'tableowner'});
	print "$tablename\t$tableowner\n";
}

$sth->finish();
$dbh->disconnect();

#����� ���������. ��� ������, ����� ���������� fetchrow_hashref �� ��������
#���, ������� �������� � �������� ������ - ����� ����� � ��� �������,
#������� ��� ������ ������, � � �������� ������ - �������� ���� ����� ���
#����� ������. ��� ������ ��� � � ���������� ������� ������������ � �����
#while, ������� �������������, ����� ������� fetchrow_hashref ����������
#������ �������� undef.
#��� ���������� �������� ���������� ������� ��� ������ � ��������� ����
#SELECT. �������, ���� ����������� DBI �� �������������. ����, ��������, ���
#������� selectrow_array, selectall_arrayref, selectcol_arrayref �
#�������������� fetchrow_arrayref, fetchall_arrayref. � �����-�� �������,
#��������, ����� ������� ��������������� ����� �� ���. ��� �� ����� � ��
#����� �� ��������� � ��������� �������� ������� � ����������� man �� DBI.

