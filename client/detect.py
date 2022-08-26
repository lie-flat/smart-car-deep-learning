import cv2
from ai import DetModel
from camera import CameraReader, CVReader
from controller import connect_to_board, buzz as buzz_raw
from functools import partial


THRESHOLD = 0.5

if __name__ == "__main__":
    model = DetModel()
    devices = connect_to_board()
    buzz = partial(buzz_raw, devices['board'])
    # camera = CVReader(480, 320)
    camera = CameraReader(devices['esp32-cam'])
    while True:
        img = camera.read()
        results = model.predict(img)
        if len(results) == 0:
            continue
        # 通过 THRESHOLD 过滤 + 转成字典（去重）
        detected = {result['class']: result
                    for result in results if result['score'] > THRESHOLD}
        for cat, result in detected.items():
            print(cat)
            cv2.rectangle(img, (result['xmin'], result['ymin']), (result['xmax'], result['ymax']), (0,0,255), 2)
            if cat == 'warning':
                buzz(2000, 500)
            elif cat == 'prohibitory':
                buzz(4000, 500)
            else:  # 'mandatory'
                buzz(6000, 500)
        cv2.imshow("detect", img)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
