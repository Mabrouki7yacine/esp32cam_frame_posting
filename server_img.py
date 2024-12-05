from flask import Flask, request, jsonify
import os

app = Flask(__name__)

# Define the upload folder and ensure it exists
UPLOAD_FOLDER = 'uploads'
os.makedirs(UPLOAD_FOLDER, exist_ok=True)

@app.route('/upload', methods=['POST'])
def upload_frame():
    if request.method == 'POST':
        # Get the binary data from the request
        image_data = request.data
        
        # Generate a unique filename based on your needs
        file_path = os.path.join(UPLOAD_FOLDER, 'frame_{}.jpg'.format(len(os.listdir(UPLOAD_FOLDER)) + 1))
        
        # Write the image data to a file
        with open(file_path, 'wb') as f:
            f.write(image_data)

        # Return a success response
        return jsonify({"message": "Image received", "file_path": file_path}), 200
    else:
        return jsonify({"message": "Method not allowed"}), 405

@app.route('/upload', methods=['GET'])
def donkey():
    return "posti ya rassa !!!!", 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
