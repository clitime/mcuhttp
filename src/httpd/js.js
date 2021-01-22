async function getVersion() {
    var vers = "";
    let response = await fetch("/get_version.cgi");

    if (response.ok) {
        vers = await response.text();
    } else {
        alert("Error HTTP: " + response.status);
    }

    return vers;
}


function init() {
    getVersion().then(function(ver) {
        let str = document.getElementById('version') ;
        str.value = "";
        str.innerHTML = 'Version: ' + ver;
    });
}


function getConfiguration() {

}
