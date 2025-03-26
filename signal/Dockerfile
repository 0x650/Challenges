FROM python:slim AS app
RUN pip install --no-cache-dir progress

FROM pwn.red/jail
COPY --from=app / /srv
COPY s /srv/app/run
COPY flag.txt /srv/app/flag
