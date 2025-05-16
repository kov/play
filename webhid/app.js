const connect_button = document.querySelector("button#connect")
var device = null;
var lastResponse = null;

window.addEventListener("load", () => {
    connect_button.addEventListener("click", async () => {
        const devices = await navigator.hid.requestDevice({
            "filters": [],
        })
        console.log(devices);
        device = devices[1];

        await device.open()

        for (let collection of device.collections) {
            // A HID collection includes usage, usage page, reports, and subcollections.
            console.log(`Usage: ${collection.usage}`);
            console.log(`Usage page: ${collection.usagePage}`);

            for (let inputReport of collection.inputReports) {
              console.log(`Input report: ${inputReport.reportId}`);
              // Loop through inputReport.items
            }

            for (let outputReport of collection.outputReports) {
              console.log(`Output report: ${outputReport.reportId}`);
              // Loop through outputReport.items
            }

            for (let featureReport of collection.featureReports) {
              console.log(`Feature report: ${featureReport.reportId}`);
              // Loop through featureReport.items
            }

            // Loop through subcollections with collection.children
        }

    })
})