function turnWhatsAppSpinner(wp_index, state) {
    const button = document.getElementById('wp'+wp_index+'-test');
    const spinner = button.querySelector('.spinner-border');
    spinner.style.display = (state) ? 'inline-block' : 'none'; 
    button.disabled = state; 
}


function testWhatsAppSettings(wp_index) {
    turnWhatsAppSpinner(wp_index, true);

    var wpNumber = document.querySelector('input[name="wp'+wp_index+'-number"]').value;
    var wpApiKey = document.querySelector('input[name="wp'+wp_index+'-apikey"]').value;

    var xhr = new XMLHttpRequest();
    xhr.open('GET', '/test?'
            + '&phone=' + wpNumber + '&apikey=' + wpApiKey, true
        , true);
  
    xhr.onload = function() {
        turnWhatsAppSpinner(wp_index, false);
        if (xhr.status >= 200 && xhr.status < 300) {
            alert('Mensaje enviado correctamente');
        } else {
            alert('Error al enviar el mensaje');
        }
    };
  
    xhr.onerror = function() {
        turnWhatsAppSpinner(wp_index, false);
        alert('Error al enviar el mensaje');
    };
  
    xhr.send();
}



document.getElementById('wp0-test').addEventListener('click', function() {
    testWhatsAppSettings(0);
});
document.getElementById('wp1-test').addEventListener('click', function() {
    testWhatsAppSettings(1);
});
document.getElementById('wp2-test').addEventListener('click', function() {
    testWhatsAppSettings(2);
});
document.getElementById('wp3-test').addEventListener('click', function() {
    testWhatsAppSettings(3);
});