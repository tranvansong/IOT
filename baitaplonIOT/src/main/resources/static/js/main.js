alertify.set('notifier', 'position', 'bottom-left');

async function fetchData(apiUrl) {
	const response = await fetch(apiUrl);
	if (!response.ok) {
		throw new Error(`Network response was not ok for ${apiUrl}`);
	}
	return await response.json();
}

function fetchAllData() {
	const apiUrls = [
		"http://localhost:8080/api/mqtt/subscribe?topic=canhBaoChay/gas&wait_millis=1000",
		"http://localhost:8080/api/mqtt/subscribe?topic=canhBaoChay/fire&wait_millis=1000",
		"http://localhost:8080/api/mqtt/subscribe?topic=canhBaoChay/pump&wait_millis=1000",
		"http://localhost:8080/api/mqtt/subscribe?topic=canhBaoChay/buzzer&wait_millis=1000"
	];

	// Gửi nhiều yêu cầu API cùng một lúc
	const promises = apiUrls.map(apiUrl => fetchData(apiUrl));

	Promise.all(promises)
		.then(dataArray => {
			console.log(dataArray);
			let gasStatus = dataArray[0][0].message;
			let fireStatus = dataArray[1][0].message;
			let pumpStatus = dataArray[2][0].message;
			let buzzerStatus = dataArray[3][0].message;

			alertify.error("Cảnh báo phát hiện cháy !!!");

			console.log(gasStatus);
			console.log(fireStatus);
			console.log(pumpStatus);
			
			
			if(fireStatus == 0) {
				document.getElementById("flame-status").textContent = "Có cháy";
				document.getElementById("flame-status").style.color = "red";
			}else {
				document.getElementById("flame-status").textContent = "An toàn";
			}
			
			if(pumpStatus == 0) {
				document.getElementById("pump-status").textContent = "Đang bật";
				document.getElementById("pump-status").style.color = "red";
			}else {
				document.getElementById("pump-status").textContent = "Tắt";
			}
			
			if (buzzerStatus == 1) {
				document.querySelector(".buzzer-on").style.display = "block";
				document.querySelector(".buzzer-off").style.display = "none";
				document.getElementById("buzzer-status").textContent = "Đang bật";
				document.getElementById("buzzer-status").style.color = "red";
			}else {
				document.querySelector(".buzzer-on").style.display = "none";
				document.querySelector(".buzzer-off").style.display = "block";
				document.getElementById("buzzer-status").textContent = "Tắt";
			}
			console.log(buzzerStatus);

			document.getElementById("gas-status").innerHTML = gasStatus;

		})
		.catch(error => {
			console.error("Error fetching data:", error);
		});
}

// Cập nhật dữ liệu mỗi 1 giây
setInterval(fetchAllData, 1000);

