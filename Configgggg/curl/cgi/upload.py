import cgi
import io
import sys
import os

def parser(request_string):
    # Split the request string into headers and body
    try:
        byte_stream = io.BytesIO(request_string.encode('utf-8', 'surrogateescape'))
        form = cgi.FieldStorage(
            fp=byte_stream,
            environ={
                'REQUEST_METHOD': sys.argv[1],
                'CONTENT_TYPE': sys.argv[2],
            },
            keep_blank_values=True
        )
        filename = form['file'].filename
        full_path = os.path.join(sys.argv[3], filename) #./docs/curl/upload
        # print(full_path)
        file = form['file'].value
        with open(full_path, 'wb') as f:
            f.write(file)
        
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)
def receive_data():
    input_data = sys.stdin.read() #cgi upload from cpp dup tmpfile to stdin.
    parser(input_data)

receive_data()