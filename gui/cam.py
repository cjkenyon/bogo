import cv2

video_file = "fifo264"
cap = cv2.VideoCapture(video_file)

if not cap.isOpened():
    print("Error: Couldn't open the video file.")
    exit()

while True:
    ret, frame = cap.read()

    if not ret:
        break

    cv2.imshow('Video', frame)

    if cv2.waitKey(25) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
