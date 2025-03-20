import pytest
from modules.ip_hash_table import HashTableIp

@pytest.fixture
def hash_table():
    return HashTableIp()

# Тест добавления и поиска IP
def test_add_and_search_ip(hash_table):
    hash_table.add("192.168.0.1", "example.com")
    hash_table.add("10.0.0.1", "test.org")

    assert hash_table.search("192.168.0.1") == "example.com"
    assert hash_table.search("10.0.0.1") == "test.org"
    assert hash_table.search("172.16.0.1") is None

# Тест добавления и поиска домена
def test_add_and_search_domain(hash_table):
    hash_table.add("192.168.0.1", "example.com")
    hash_table.add("10.0.0.1","test.org")

    assert hash_table.search("example.com") == "192.168.0.1"
    assert hash_table.search("test.org") == "10.0.0.1"
    assert hash_table.search("unknown.com") is None

# Тест удаления IP
def test_delete_ip(hash_table):
    hash_table.add("192.168.0.1", "example.com")
    hash_table.delete_ip("192.168.0.1")

    assert hash_table.search("192.168.0.1") is None

def test_delete_domain(hash_table):
    hash_table.add("192.168.0.1","example.com")
    hash_table.delete_domain("example.com")

    assert hash_table.search("example.com") is None

# Тест удаления пары
def test_delete_pair(hash_table):
    hash_table.add("192.168.0.1", "example.com")
    hash_table.add("10.0.0.1", "test.org")

    hash_table.delete_pair("192.168.0.1")
    assert hash_table.search("192.168.0.1") is None
    assert hash_table.search("10.0.0.1") == "test.org"

    hash_table.delete_pair("test.org")
    assert hash_table.search("10.0.0.1") is None

# Тест печати таблицы
def test_print_table(hash_table, capsys):
    hash_table.add("192.168.0.1", "example.com")
    hash_table.add("10.0.0.1", "test.org")

    hash_table.print()
    captured = capsys.readouterr()

    assert "192.168.0.1" in captured.out
    assert "example.com" in captured.out
    assert "10.0.0.1" in captured.out
    assert "test.org" in captured.out
