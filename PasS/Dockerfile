FROM python:3.10-slim

# Avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Set working directory
WORKDIR /app

# Install required packages: bash, ping, build tools (if needed)
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        bash \
        iputils-ping \
        gcc \
        libffi-dev \
        libssl-dev \
        && apt-get clean && rm -rf /var/lib/apt/lists/*

# Copy app files
COPY flag.txt .
COPY app.py .

# Make files read-only and owned by root
RUN chmod 444 flag.txt app.py && chown root:root flag.txt app.py

# Install Python dependencies
RUN pip install --no-cache-dir flask gunicorn

USER nobody

# Expose the app port
EXPOSE 5000

# Run the app with Gunicorn
CMD ["gunicorn", "-w", "4", "-b", "0.0.0.0:5000", "app:app"]
