#include <iomanip>
#include <iostream>
#include <sstream>

#include <vsomeip/vsomeip.hpp>

#define SAMPLE_SERVICE_ID 0x1234
#define SAMPLE_INSTANCE_ID 0x5678
#define SAMPLE_METHOD_ID 0x0421

std::shared_ptr<vsomeip::application> app;

void on_message(const std::shared_ptr<vsomeip::message> &_request)
{

    std::shared_ptr<vsomeip::payload> its_payload = _request->get_payload();
    vsomeip::length_t l = its_payload->get_length();

    // Print the two numbers received
    if (l >= 2)
    {
        int num1 = *(its_payload->get_data());
        int num2 = *(its_payload->get_data() + 1);
        std::cout << "The secret formula is: " << num1 << " " << num2 << std::endl;
    }

    // Calculate sum
    int sum = 0;
    for (vsomeip::length_t i = 0; i < l; i++)
    {
        sum += *(its_payload->get_data() + i);
    }

    // Create response
    std::shared_ptr<vsomeip::message> its_response = vsomeip::runtime::get()->create_response(_request);
    std::shared_ptr<vsomeip::payload> its_payload_response = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data_response;

    // Check sum and print message
    if (sum == 5)
    {
        its_payload_data_response.push_back(1);
        std::cout << "Toggling CAPSLOCK LED" << std::endl;
        its_payload_response->set_data(its_payload_data_response);
        its_response->set_payload(its_payload_response);
    }
    else
    {
        its_payload_data_response.push_back(0);
        std::cout << "The sauce is sus" << std::endl;
        its_payload_response->set_data(its_payload_data_response);
        its_response->set_payload(its_payload_response);
    }

    app->send(its_response);
}

int main()
{

    app = vsomeip::runtime::get()->create_application("CAPSLOCK TOGGLE");
    app->init();
    app->register_message_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_METHOD_ID, on_message);
    app->offer_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    app->start();
}