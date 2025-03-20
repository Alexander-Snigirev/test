import pytest
from modules.ip_hash_table import is_ip, ip_to_int, ip_hash,fnv1a_hash

# Тест функции is_ip
@pytest.mark.parametrize("ip,expected", [
    ("192.168.0.1", True),
    ("10.0.0.1", True),
    ("example.com", False),
    ("invalid_ip", False),
])
def test_is_ip(ip, expected):
    assert is_ip(ip) == expected

# Тест функции ip_to_int
def test_ip_to_int():
    assert ip_to_int("192.168.0.1") == 3232235521
    assert ip_to_int("10.0.0.1") == 167772161

# Тест функции ip_hash
def test_ip_hash():
    table_size = 1009
    assert ip_hash("192.168.0.1", table_size) == ip_to_int("192.168.0.1") % table_size
    assert ip_hash("10.0.0.1", table_size) == ip_to_int("10.0.0.1") % table_size

# Тест функции fnv1a_hash
def test_fnv1a_hash():
    table_size = 1009
    domain = "example.com"
    result = fnv1a_hash(domain, table_size)
    assert isinstance(result, int)
    assert 0 <= result < table_size
