#!/usr/bin/perl
use DBI;

# ��� ���� ������
$dbname = "CupsLog";
# ��� ������������
$username = "postgres";
# ������
$password = "pg";
# ��� ��� IP ����� �������
$dbhost = "localhost";
# ����
$dbport = "5432";
# �����
$dboptions = "-e";
# ��������
$dbtty = "ansi";


$dbh = DBI->connect("dbi:PgPP:dbname=$dbname;host=$dbhost;port=$dbport;options=$dboptions;tty=$dbtty","$username","$password",{PrintError => 0});

if ($DBI::err != 0) {
	print $DBI::errstr."\n";
	exit($DBI::err);
}

$dbh->disconnect();
