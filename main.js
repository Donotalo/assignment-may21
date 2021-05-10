const yargs = require('yargs/yargs');
const { hideBin } = require('yargs/helpers');
const { HeiConnectInterface } = require('./hei_connect_interface/hei_connect_interface');

/**
 * Supported command by the module.
 * */
const HEI_COMMAND = 'hc [<' + HeiConnectInterface.GetApi()[0] + '> <' + HeiConnectInterface.GetApi()[1] + '>]';

/**
 * A command builder & handler.
 * */
const argv = yargs(hideBin(process.argv))
    .command(

        // Command
        HEI_COMMAND,

        // Command description
        'Control the Hei Connect',

        // Description of the positionals
        (yargs) => {

            yargs.positional(HeiConnectInterface.GetApi()[1], {

                describe: 'Sets the rotation speed in RPM',
                type: 'number'
            })
            .positional(HeiConnectInterface.GetApi()[0], {

                describe: 'Gets currently set rotation speed in RPM'
            })
        },

        // Command handler
        (argv) => {

            if (HeiConnectInterface.GetApi()[1] in argv &&
                argv[HeiConnectInterface.GetApi()[1]] !== null &&
                argv[HeiConnectInterface.GetApi()[1]] !== undefined) {

                HeiConnectInterface.SetSpeed(argv[HeiConnectInterface.GetApi()[1]]);
            }
            else if (HeiConnectInterface.GetApi()[0] in argv) {

                HeiConnectInterface.GetSpeed();
            }
        }
    )
    .argv;

//console.log("Device Interfaces");
//console.log(argv);
