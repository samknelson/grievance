(function($) { $(document).ready(function () {

	sirius_stripe_payment_widget_init();

	function sirius_stripe_payment_widget_init() {
		sirius_stripe_payment_widget_spinner_on();

		console.log(Drupal.settings.sirius_ledger_stripe);
		const stripe = Stripe(Drupal.settings.sirius_ledger_stripe.account_id);
		const options = {
		  clientSecret: Drupal.settings.sirius_ledger_stripe.client_secret,
	  	appearance: {
	  		theme: 'stripe'
	  	},
		};

		const elements = stripe.elements(options);
		const paymentElement = elements.create('payment');
	  paymentElement.on("ready", function(){
	    sirius_stripe_payment_widget_spinner_off();
	  })
		paymentElement.mount('#payment-element');

		const form = document.getElementById('payment-form');
		console.log(form);
		form.addEventListener('submit', async (event) => {
		  event.preventDefault();

		  const {error} = await stripe.confirmSetup({
		    //`Elements` instance that was used to create the Payment Element
		    elements,
		    confirmParams: {
		      return_url: Drupal.settings.sirius_ledger_stripe.return_url,
		    }
		  });

		  if (error) {
		    // This point will only be reached if there is an immediate error when
		    // confirming the payment. Show error to your customer (for example, payment
		    // details incomplete)
		    const messageContainer = document.querySelector('#error-message');
		    messageContainer.textContent = error.message;
		  } else {
		    // Your customer will be redirected to your `return_url`. For some payment
		    // methods like iDEAL, your customer will be redirected to an intermediate
		    // site first to authorize the payment, then redirected to the `return_url`.
		  }
		});

	}

	function sirius_stripe_payment_widget_spinner_on() {
		$('.sirius_stripe_wrapper').addClass('loading');
		$('.sirius_stripe_wrapper .sirius_stripe_loading').show();
		$('.sirius_stripe_wrapper #payment-form').hide();
	}

	function sirius_stripe_payment_widget_spinner_off() {
		$('.sirius_stripe_wrapper').removeClass('loading');
		$('.sirius_stripe_wrapper .sirius_stripe_loading').hide();
		$('.sirius_stripe_wrapper #payment-form').show();
	}

}); }(jQuery));
