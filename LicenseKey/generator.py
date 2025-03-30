from itertools import product
import random

# Originally I wanted all the coefficients to be 1 such that it would look nicer
# However that would always lead to a singular matrix
# So I threw in coefficients later as a mix
# The function is overcomplicated since it could have been random.randint(-10, 10)
# But I am not gonna bother with this anymore lmao
def generate_expressions(numbers):
    ops = ['+', '-']
    n = len(numbers)
    results = []

    for combination in product(ops, repeat=n-1):
        if len(results) > (n ** 4): # if there are way too many combinations limit it to n ^ 4
            break
        expression = str(numbers[0]) 
        i = 0
        return_expr = f"key[{i}]"
        for num, op in zip(numbers[1:], combination):
            i += 1
            coff = random.randint(0, 10)
            expression += f" {op} {coff} * {num}"
            return_expr += f" {op} {coff} * key[{i}]"

        result = eval(expression)
        results.append((expression, result, return_expr))
    
    return results

flag = "{FLAG}"
numbers = [ord(c) for c in flag]
expressions = generate_expressions(numbers)

final_list = []

for expr, value, return_expr in expressions:
    s = f"&& ({return_expr} == {value})"
    final_list.append(s)

final = random.sample(final_list, len(flag))
k = ""
for f in final:
    k += f
print(k)
