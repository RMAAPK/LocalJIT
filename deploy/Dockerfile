FROM python:3.11-alpine

WORKDIR /app

RUN apk add --no-cache curl

COPY server/ /app/server/

ENV PYTHONUNBUFFERED=1
ENV PORT=7470
ENV STORAGE_PATH=/data/vault

EXPOSE 7470

CMD ["python3", "-u", "/app/server/server.py"]
