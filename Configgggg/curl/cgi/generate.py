import os

def generate_html():
    html = """
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>File Upload</title>
        <script>
            async function deleteFile(file) {
                const formData = new FormData();
                formData.append('file', file);
                const response = await fetch('/delete', {
                    method: 'DELETE',
                    body: formData
                }).then(response => response.status == 200 ? location.reload() : alert('Error deleting file'));
            }

        
            async function uploadFile() {
                const formData = new FormData();
                const input = document.querySelector('input[type="file"]');
                formData.append('file', input.files[0]);
                const response = await fetch('/upload', {
                    method: 'POST',
                    body: formData
                });
                if (response.status == 201) {
                    location.reload();
                } else if (response.status == 413) {
                    window.location.href = '../default-error/413.html'; // Redirect to the 413 error page
                } else {
                    alert('Error uploading file');
                }
            }

            function goBack() {
                window.location.href = window.location.origin + '/';
            }
        </script>
    </head>
    <body>
        <h1>File Upload</h1>
        <form action="/upload" method="post" enctype="multipart/form-data">
            <input type="file" name="file">
            <input type="button" value="Upload" onclick="uploadFile()">
        </form>
        
        <h2>Uploaded Files:</h2>
        <ul>
    """

    for root, dirs, files in os.walk(os.path.abspath("docs/curl/upload")):
        if files.__len__() > 0:
            for file in files:
                html += f"<li><input type=\"button\" value=\"delete\" onclick=\"deleteFile('{file}')\" > <a href='./upload/{file}'> {file} </a></li>"

    html += """
        </ul>
        <button onclick="goBack()">Go Back</button>
    </body>
    </html>
    """
    
    return html

def list_files(directory):
    files = os.listdir(directory)
    return files

def main():

    html_content = generate_html()
    print(html_content)

if __name__ == '__main__':
    main()


            # async function uploadFile() {
            #     const formData = new FormData();
            #     const input = document.querySelector('input[type="file"]');
            #     formData.append('file', input.files[0]);
            #     const response = await fetch('/upload', {
            #         method: 'POST',
            #         body: formData
            #     }).then(response => response.status == 201 ? location.reload() : alert('Error uploading file'));
            # }
