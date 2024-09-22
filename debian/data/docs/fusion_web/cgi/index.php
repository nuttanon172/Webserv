<?php
// header("HTTP/1.1 200 OK");
header("Content-Type: text/html");
header_remove("X-Powered-By");
header_remove("Content-type");
// phpinfo();

// echo $dir = __DIR__;
$defaultDir =  __DIR__;;
$defaultfol = dirname($defaultDir, 1);
$nowdir = $defaultfol.$_GET['dir'];
$dir = isset($nowdir) ? $nowdir : $defaultDir;
// echo $dir;
$selectedDir = $dir;
$parentDir = dirname($selectedDir);
// Function to get directories
function getDirectories($dir) {
    $directories = [];
    $items = scandir($dir);
    foreach ($items as $item) {
        if ($item != '.' && $item != '..') {
            $path = $dir . '/' . $item;
            if (is_dir($path)) {
                $directories[] = $path;
            }
        }
    }
    return $directories;
}

// Function to list files and directories
function listFiles($dir) {
    $items = scandir($dir);
    echo '<ul>';
    foreach ($items as $item) {
        if ($item == '.' || $item == '..') continue;
        $path = $dir . '/' . $item;
        echo '<li style="position: relative; display: flex; justify-content: space-between;">';
        if (is_dir($path)) {
            echo '<a href="?dir=' . urlencode($path) . '">' . htmlspecialchars($item) . '</a>';
        } else {
            echo '<a href="' . htmlspecialchars($path) . '" download>' . htmlspecialchars($item) . '</a>';
        }
        echo '<form id="deleteForm-' . urlencode($path) . '" method="POST" style="margin: 0;">
                <input type="hidden" name="delete_path" value="' . htmlspecialchars($path) . '">
                <button type="submit" class="delete-button" name="_method" value="DELETE">Delete</button>
              </form>';
        echo '</li>';
    }
    echo '</ul>';
}

// Handle POST requests for folder creation, file upload, and deletion
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // echo 'hello';
    if (isset($_POST['new_folder']) && $_POST['new_folder'] != '') {
        $newFolderName = urldecode($_POST['folder_name']);
        if (!empty($newFolderName)) {
            mkdir($selectedDir . '/' . $newFolderName, 0777, true);
        }
    } elseif (isset($_FILES['upload_file'])) {
        $uploadFile = $selectedDir . '/' . basename($_FILES['upload_file']['name']);
        move_uploaded_file($_FILES['upload_file']['tmp_name'], $uploadFile);
	} elseif (isset($_GET['delete_path'])){
		$deletePath = urldecode($_GET['delete_path']);
		if (file_exists($deletePath)) {
			if (is_dir($deletePath)) {
				// ลบไดเรกทอรี (ต้องเป็นไดเรกทอรีที่ว่างเปล่า)
				if (rmdir($deletePath)) {
					echo "ลบไดเรกทอรีเรียบร้อย";
				} else {
					echo "ไม่สามารถลบไดเรกทอรีได้";
				}
			} else {
				// ลบไฟล์
				if (unlink($deletePath)) {
					echo "ลบไฟล์เรียบร้อย";
				} else {
					echo "ไม่สามารถลบไฟล์ได้";
				}
			}
		} else {
			echo "ไฟล์หรือไดเรกทอรีไม่พบ";
		}
	}
}

?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>File Explorer</title>
    <style>
        body { font-family: Arial, sans-serif; }
        ul { list-style-type: none; padding-left: 20px; }
        li { margin: 5px 0; }
        .header {
            display: flex;
            flex-direction: column;
            align-items: flex-start;
            margin-bottom: 10px;
        }
        .header h1 {
            margin: 0;
        }
        .button-wrapper {
            display: flex;
            margin-top: 10px;
        }
        button {
            padding: 5px 10px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            font-size: 14px;
            display: inline-block;
            text-align: center;
            margin-right: 5px;
        }
        .new-button {
            background-color: #007bff;
            color: white;
        }
        .new-button:hover {
            background-color: #0056b3;
        }
        .back-button {
            background-color: #6c757d;
            color: white;
        }
        .back-button:hover {
            background-color: #5a6268;
        }
        .delete-button {
            background-color: #dc3545;
            color: white;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            font-size: 14px;
        }
        .delete-button:hover {
            background-color: #c82333;
        }
    </style>
</head>
<body>

<div class="header">
    <h1>File Explorer</h1>
    <div class="button-wrapper">
        <button class="new-button" onclick="toggleNewMenu()">New</button>
        <?php if ($selectedDir !== $defaultDir): ?>
            <form method="GET" action="">
                <input type="hidden" name="dir" value="<?php echo htmlspecialchars($parentDir); ?>">
                <button type="submit" class="back-button">Back</button>
            </form>
        <?php endif; ?>
    </div>
</div>

<div id="new-menu" style="display: none;">
    <form method="POST" style="margin-bottom: 10px;">
        <input type="text" name="folder_name" placeholder="New folder name" required>
        <button type="submit" name="new_folder" value="true" class="new-button">Create Folder</button>
    </form>
    <form method="POST" enctype="multipart/form-data">
        <input type="file" name="upload_file" required>
        <button type="submit" class="new-button">Upload File</button>
    </form>
</div>

<?php
// echo ini_get('open_basedir');

// echo 'Selected directory: ' . $selectedDir;

// if (is_readable($selectedDir)) {
//     echo 'Directory is readable.';
// } else {
//     echo 'Directory is not readable.';
// }

// if (file_exists($selectedDir)) {
//     echo 'Directory exists.';
// } else {
//     echo 'Directory does not exist.';
// }

if (is_dir($selectedDir) && is_readable($selectedDir)) {
    listFiles($selectedDir);
} else {
    echo 'Invalid directory or permission denied.';
}
?>

<script>
function toggleNewMenu() {
    var menu = document.getElementById("new-menu");
    menu.style.display = (menu.style.display === "none") ? "block" : "none";
}

document.querySelectorAll('form[id^="deleteForm-"]').forEach(form => {
    form.addEventListener('submit', function(event) {
        event.preventDefault(); // Prevent default form submission

        const formData = new FormData(form);
        fetch(form.action, {
            method: 'DELETE',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: new URLSearchParams(formData).toString()
        })
        .then(response => response.text())
        .then(data => {
            console.log(data);
            alert('Delete operation successful');
            location.reload(); // Refresh the page
        })
        .catch(error => console.error('Error:', error));
    });
});
</script>

</body>
</html>
