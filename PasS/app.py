from flask import Flask, request, render_template_string
import os
import re
import subprocess

app = Flask(__name__)

HTML_TEMPLATE = """
<style>
@import url('https://fonts.googleapis.com/css2?family=JetBrains+Mono:ital,wght@0,100..800;1,100..800&display=swap');
* {
    font-family: "JetBrains Mono", monospace;
    font-optical-sizing: auto;
    font-weight: <weight>;
    font-style: normal;
}
</style>
<title>Ping as a Service</title>
<h1>Ping as a Service</h1>
<form method=post>
  <input name=target placeholder="Enter IP (Only IPv4)" required>
  <input type=submit value=Ping>
</form>
{% if result %}
<h2>Ping Result:</h2>
<pre>{{ result }}</pre>
{% endif %}
"""

@app.route('/', methods=['GET', 'POST'])
def index():
    result = None
    if request.method == 'POST':
        target = request.form['target']

        if re.search('[a-zA-Z]', target):
            return render_template_string(HTML_TEMPLATE, result="Invalid IP")
        
        cmd = f"bash -c \"ping -c 2 {target}\""
        try:
            result = subprocess.check_output(cmd, shell=True, stderr=subprocess.STDOUT, text=True)
        except subprocess.CalledProcessError as e:
            result = e.output

    return render_template_string(HTML_TEMPLATE, result=result)

if __name__ == '__main__':
    app.run()

