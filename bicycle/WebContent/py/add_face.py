import sys
import face_recognition
import numpy as np

def add_face(img_path,pick_path):
    img = face_recognition.load_image_file(img_path)
    encode = face_recognition.face_encodings(img)[0]
    np.savetxt(pick_path, encode, delimiter=",")
    # encode.dump(open(pick_path,"wb"))
    # print(encode)

if __name__ == "__main__":
    # assert(len(sys.argv)>=3)
    img_path = sys.argv[1]
    pick_path = sys.argv[2]
    add_face(img_path,pick_path)
