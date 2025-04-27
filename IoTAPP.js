//Programacion necesaria con los indentificadores

/*
AEJE 1957302
DISEÑO DE SISTEMAS EMBEBIDOS
*/

// Your web app's Firebase configuration
const firebaseConfig = {
  apiKey: "AIzaSyAcQny59ysfpBkhQ3xGjyztIeDJcFo0G4o",
  authDomain: "iotapp-5608d.firebaseapp.com",
  databaseURL: "https://iotapp-5608d-default-rtdb.firebaseio.com",
  projectId: "iotapp-5608d",
  storageBucket: "iotapp-5608d.firebasestorage.app",
  messagingSenderId: "674127551078",
  appId: "1:674127551078:web:8b0f3b61c89682af1caba7"
};

// Initialize Firebase
firebase.initializeApp(firebaseConfig);
var dbRef = firebase.database();
// 1. Path for 8-digit number
// Referencia correcta al número de 8 dígitos
var dbDisplayNumber = dbRef.ref("/ESP32IoTAPP_DISPLAY/DISPLAY");

// Mostrar el número actual en el input
dbDisplayNumber.on("value", function(snapshot) {
  const currentVal = snapshot.val();
  console.log("Valor actual de DISPLAY:", currentVal);
  document.getElementById("displaySegment").value = currentVal || "";
});

// Enviar un nuevo número a Firebase
function captureNumber(inputId) {
  const val = document.getElementById(inputId).value;
  const num = parseInt(val);

  if (!isNaN(num) && num >= 0 && num <= 99999999) {
    dbDisplayNumber.set(num)
      .then(() => console.log("Número enviado:", num))
      .catch(error => console.error("Error al enviar:", error));
  } else {
    alert("Por favor, ingrese un número válido de hasta 8 dígitos.");
  }
}

