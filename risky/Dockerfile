FROM python:slim AS app
RUN pip install --no-cache-dir progress

FROM pwn.red/jail
COPY --from=app / /srv
COPY risky /srv/app/risky
COPY qemu-riscv64 /srv/app/qemu-riscv64
COPY run /srv/app/run
COPY flag.txt /srv/app/flag
