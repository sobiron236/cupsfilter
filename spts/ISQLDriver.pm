# ��� ���������. ��� ������� ���������� ������ ���� �����������.
# ����� - ���-�� �������. 
package ISQLDriver;
use Carp;
use strict;

our $VERSION   = '0.01';
$VERSION = eval $VERSION;
our $AUTHORITY = 'freelance:SLANT';
#TODO �������� ������� ��������� �� ������ � ����� ���� ���������� ���������� ������ ?
sub connect { 
	croak ("I'm pure!"); 
};
sub select { 
	croak "I'm pure!" 
}
sub insert { 
	croak "I'm pure!" 
}

sub update { 
	croak "I'm pure!" 
}

sub delete { 
	croak "I'm pure!" 
}

sub disconnect { 
	croak "I'm pure!" 
}
1;
