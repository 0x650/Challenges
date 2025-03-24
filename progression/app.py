from flask import Flask, session, request
from datetime import timedelta
from random import randint

TIMEOUT = 10

FLAG = "{FLAG}"

class Series:
    a = 0
    b = 0
    c = 0

    def __init__(self, a, b, c):
        self.a = a
        self.b = b
        self.c = c
        
    def general_term(self, x):
        return self.a * (x ** 2) + self.b * (x) + self.c
    
    def sum_to_x_terms(self, x):
        squared_sum = x * (x + 1) * ((2 * x) + 1)
        squared_sum = squared_sum / 6

        n_sum = x * (x + 1)
        n_sum = n_sum / 2

        return int(self.a * squared_sum + self.b * n_sum + self.c * x)
    
app = Flask(__name__)
app.secret_key = FLAG
app.permanent_session_lifetime = timedelta(seconds=TIMEOUT)

@app.route('/')
def index():
    session.permanent = True
    
    n = [randint(0, 100), randint(0, 100), randint(0, 100)]
    session['series'] = n
    
    output = "Given the series<br><br>"
    s = Series(n[0], n[1], n[2])
    for i in range(1, 11):
        output += str(s.general_term(i)) + " "
    output += f"<br><br>Calculate the sum of the first 100 terms within {TIMEOUT} seconds :)"
    output += "<br><br>Submit your solution at /submit/your_solution"

    return output

@app.route('/submit/', methods=['GET'])
def submit_no_solution():
    return "Please provide a solution. Use /submit/your_solution to submit your answer."

@app.route('/submit/<solution>', methods=['GET'])
def submit(solution):
    if 'series' not in session:
        return "Oops the session has expired!"
    
    n = session['series']
    s = Series(n[0], n[1], n[2])

    try:
        sol = int(solution)
    except:
        return "The solution entered must be an integer!"
    
    ans = s.sum_to_x_terms(100)

    session.clear()

    # Due to floating point errors take the difference. If it is small enough then we have a good enough answer.
    if (ans - sol > 1) or (ans - sol < -1):
        return f"Oops you got it wrong. Try again!<br><br>Wanted {ans} got {sol}"
    
    return f"Congrats you got it right! Here's your flag {FLAG}"
    

if __name__ == '__main__':
    app.run()

