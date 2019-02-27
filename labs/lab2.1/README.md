Lab 2.1 - ClockWall
===================

To compile:

- go build clock2.go
- go build clockWall.go

Then run the clocks servers you want in the format:

- TZ=TIMEZONE ./clock2 -port PORT

Example:

- $ TZ=US/Eastern    ./clock2 -port 8010 &
- $ TZ=Asia/Tokyo    ./clock2 -port 8020 &
- $ TZ=Europe/London ./clock2 -port 8030 &

Then run the clockWall in format:
- $ clockWall City=localhost:PORT

Example:

- $ clockWall NewYork=localhost:8010 Tokyo=localhost:8020 London=localhost:8030
