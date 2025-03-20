def rabin_karp(pattern: str, text: str):
    p = 10**9 + 7  
    x = 263

    m = len(pattern)
    n = len(text)

    pattern_hash = 0
    for i in range(m):
        pattern_hash = (pattern_hash * x + ord(pattern[i])) % p

    x_pow_m = 1
    for _ in range(m - 1):
        x_pow_m = (x_pow_m * x) % p

    current_hash = 0
    for i in range(m):
        current_hash = (current_hash * x + ord(text[i])) % p

    occurrences = []
    for i in range(n - m + 1):
        if current_hash == pattern_hash:
            if text[i:i + m] == pattern:
                occurrences.append(i)

        if i < n - m:
            current_hash = (current_hash - ord(text[i]) * x_pow_m) % p
            current_hash = (current_hash * x + ord(text[i + m])) % p
            current_hash = (current_hash + p) % p  
    return occurrences