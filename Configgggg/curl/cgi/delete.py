import cgi
import io
import sys
import os

def parse_multipart_form_data(request_string):
    # Split the request string into headers and body
    try:
        # print(len(request_string))
        byte_stream = io.BytesIO(request_string.encode('utf-8', 'surrogateescape'))

        # print(request_string.earse(0, 2))
        # print(type(byte_stream))
        form = cgi.FieldStorage(
            fp=byte_stream,
            environ={
                'REQUEST_METHOD': sys.argv[1],
                'CONTENT_TYPE': sys.argv[2],
            },
            keep_blank_values=True
        )
        full_path = os.path.join(sys.argv[3], form.getvalue('file'))
        if(os.path.exists(full_path)):
            os.remove(full_path)
        else:
            exit(1)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)
def receive_data():
    input_data = sys.stdin.read()
    parse_multipart_form_data(input_data)

receive_data()
