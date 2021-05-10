const hc_addon = require('hei_connect');

/**
 * This class acts as an interface to Hei Connect device.
 * */
class HeiConnectInterface {

    /**
     * Returns an array containing list of supported API by the device.
     * */
    static GetApi() {

        return [
            'get-speed',
            'set-speed'
        ];
    }

    /**
     * Retrieves current stirrer speed in RPM.
     * */
    static async GetSpeed() {

        console.log('Retrieving speed...');
        const success = hc_addon(
            this.GetApi()[0]
        );

        if (success) {

            console.log('Success');
        }
        else {

            console.log('Failed');
        }
    }

    /**
     * Sets stirrer speed in RPM.
     * @param {speed_rpm} A positive integer representing speed in RPM. Must be a value from [0, 1400].
     */
    static async SetSpeed(speed_rpm) {

        if (speed_rpm !== null && speed_rpm !== undefined) {

            const srpm = parseInt(speed_rpm);

            if (srpm !== NaN && srpm >= 0 && srpm <= 1400) {

                console.log('Setting speed @ %d...', srpm);
                const success = hc_addon(
                    this.GetApi()[1],
                    srpm
                );

                if (success) {

                    console.log('Success');
                }
                else {

                    console.log('Failed');
                }
            }
            else {

                console.log("Invalid parameter " + srpm);
            }
        }
        else {

            console.log('\'' + speed_rpm + '\' is not a valid input');
        }
    }
}

module.exports =
{
    HeiConnectInterface: HeiConnectInterface
};
