var colors = require('colors');
var util = require('util');

/* CONFIGURATION */
var config = {
	gcpProjectId: 'home-155111',
	gcpPubSubSubscriptionName: 'StoreToDatastore',
	gcpServiceAccountKeyFilePath: './gcp_private_key.json'
}
_checkConfig();
/* END CONFIGURATION */
console.log(colors.magenta('Authenticating with Google Cloud...'))
var gcloud = require('google-cloud')({
    projectId: config.gcpProjectId,
	keyFilename: config.gcpServiceAccountKeyFilePath,
});
console.log(colors.magenta('Authentication successful!'))


var datastore = gcloud.datastore();
var pubsub = gcloud.pubsub();


var subscription = pubsub.subscription(config.gcpPubSubSubscriptionName);


function storeEvent(message) {
    var key = datastore.key('ParticleEvent');

    datastore.save({
        key: key,
        data: _createEventObjectForStorage(message)
    }, function(err) {
		if(err) {
			console.log(colors.red('There was an error storing the event'), err);
		}
		console.log(colors.green('Particle event stored in Datastore!\r\n'), _createEventObjectForStorage(message, true))
    });

};

subscription.on('message', function(message) {
	console.log(colors.cyan('Particle event received from Pub/Sub!\r\n'), _createEventObjectForStorage(message, true));
	// Called every time a message is received.
	// message.id = ID used to acknowledge its receival.
	// message.data = Contents of the message.
	// message.attributes = Attributes of the message.
	storeEvent(message);
	message.ack();
});

function _checkConfig() {
	if(config.gcpProjectId === ''  || !config.gcpProjectId) {
		console.log(colors.red('You must set your Google Cloud Platform project ID in app`.js'));
		process.exit(1);
	}
	if(config.gcpPubSubSubscriptionName === '' || !config.gcpPubSubSubscriptionName) {
		console.log(colors.red('You must set your Google Cloud Pub/Sub subscription name in app.js'));
		process.exit(1);
	}
};

function _createEventObjectForStorage(message, log) {
	sensors = message.data.trim().split(" ")
	var obj = {
		gc_pub_sub_id: message.id,
		device_id: message.attributes.device_id,
		event: message.attributes.event,
		sensor0: sensors[0],
		sensor1: sensors[1],
		sensor2: sensors[2],
		sensor3: sensors[3],
		sensor4: sensors[4],
		sensor5: sensors[5],
		sensor6: sensors[6],
		sensor7: sensors[7],
		published_at: message.attributes.published_at
	}

	if(log) {
		return colors.grey(util.inspect(obj));
	} else {
		return obj;
	}
};
