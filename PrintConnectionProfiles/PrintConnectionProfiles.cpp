﻿// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include <string>

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Networking.Connectivity.h>

#include <wil/com.h>
#include <wil/resource.h>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Networking::Connectivity;

#include "ToString.hpp"

std::wstring PrintHeader(_In_ PCWSTR header)
{
    const auto headerLength = wcslen(header);
    const auto dashCount = headerLength + 5;
    const std::wstring dashes(dashCount, L'-');
    const std::wstring tabs(tabs_to_wstring(1));
    return tabs + dashes + L"\n" + tabs + header + L"\n" + tabs + dashes + L"\n";
}

int main()
try
{
    const auto coinit = wil::CoInitializeEx();
    const auto profiles = NetworkInformation::GetConnectionProfiles();
    unsigned iterationCount = 0;
    for (const auto& profile : profiles)
    {
        ++iterationCount;
        wprintf(L"--------------------------------- Profile # %lu ---------------------------------\n", iterationCount);

        std::wstring profileString;
        profileString.append(tabs_to_wstring(1));
        profileString.append(L"ProfileName: ").append(profile.ProfileName()).append(L"\n");
        profileString.append(tabs_to_wstring(1));
        profileString.append(L"ServiceProviderGuid: ").append(to_wstring(profile.ServiceProviderGuid())).append(L"\n");
        profileString.append(tabs_to_wstring(1));
        profileString.append(L"NetworkConnectivityLevel: ").append(to_wstring(profile.GetNetworkConnectivityLevel())).append(L"\n");
        profileString.append(tabs_to_wstring(1));
        profileString.append(L"DomainConnectivityLevel: ").append(to_wstring(profile.GetDomainConnectivityLevel())).append(L"\n");
        profileString.append(tabs_to_wstring(1));
        profileString.append(L"NetworkNames\n");
        unsigned networkNameCount = 0;
        for (const auto& networkNames : profile.GetNetworkNames())
        {
            ++networkNameCount;
            profileString.append(tabs_to_wstring(1));
            profileString.append(to_wstring(networkNameCount)).append(L". ").append(networkNames).append(L"\n");
        }
        wprintf(L"%ws\n", profileString.c_str());
        profileString.clear();

        try
        {
            profileString.append(PrintHeader(L"ConnectionCost"));
            const auto cost = profile.GetConnectionCost();
            profileString.append(tabs_to_wstring(2));
            profileString.append(L"ApproachingDataLimit: ").append(to_wstring(cost.ApproachingDataLimit())).append(L"\n");
            profileString.append(tabs_to_wstring(2));
            profileString.append(L"OverDataLimit: ").append(to_wstring(cost.OverDataLimit())).append(L"\n");
            profileString.append(tabs_to_wstring(2));
            profileString.append(L"Roaming: ").append(to_wstring(cost.Roaming())).append(L"\n");
            profileString.append(tabs_to_wstring(2));
            profileString.append(L"BackgroundDataUsageRestricted: ").append(to_wstring(cost.BackgroundDataUsageRestricted())).append(L"\n");
            profileString.append(tabs_to_wstring(2));
            profileString.append(L"NetworkCostType: ").append(to_wstring(cost.NetworkCostType())).append(L"\n");
        }
        catch (hresult_error const& ex)
        {
            profileString.append(exception_to_wstring(ex));
        }
        wprintf(L"%ws\n", profileString.c_str());
        profileString.clear();

        try
        {
            profileString.append(PrintHeader(L"DataPlanStatus"));
            const auto dataplanStatus = profile.GetDataPlanStatus();
            profileString.append(tabs_to_wstring(2));
            profileString.append(L"DataLimitInMegabytes: ").append(to_wstring(dataplanStatus.DataLimitInMegabytes())).append(L"\n");
            profileString.append(tabs_to_wstring(2));
            profileString.append(L"InboundBitsPerSecond: ").append(to_wstring(dataplanStatus.InboundBitsPerSecond())).append(L"\n");
            profileString.append(tabs_to_wstring(2));
            profileString.append(L"OutboundBitsPerSecond: ").append(to_wstring(dataplanStatus.OutboundBitsPerSecond())).append(L"\n");
            profileString.append(tabs_to_wstring(2));
            profileString.append(L"MaxTransferSizeInMegabytes: ").append(to_wstring(dataplanStatus.MaxTransferSizeInMegabytes())).append(L"\n");
            profileString.append(tabs_to_wstring(2));
            profileString.append(L"NextBillingCycle: ").append(to_wstring(dataplanStatus.NextBillingCycle())).append(L"\n");
        }
        catch (hresult_error const& ex)
        {
            profileString.append(exception_to_wstring(ex));
        }
        wprintf(L"%ws\n", profileString.c_str());
        profileString.clear();

        try
        {
            profileString.append(PrintHeader(L"NetworkSecuritySettings"));
            const auto securitySettings = profile.NetworkSecuritySettings();
            profileString.append(tabs_to_wstring(2));
            profileString.append(L"NetworkAuthenticationType: ").append(to_wstring(securitySettings.NetworkAuthenticationType())).append(L"\n");
            profileString.append(tabs_to_wstring(2));
            profileString.append(L"NetworkEncryptionType: ").append(to_wstring(securitySettings.NetworkEncryptionType())).append(L"\n");
        }
        catch (hresult_error const& ex)
        {
            profileString.append(exception_to_wstring(ex));
        }
        wprintf(L"%ws\n", profileString.c_str());
        profileString.clear();

        try
        {
            profileString.append(PrintHeader(L"NetworkAdapter"));
            profileString.append(to_wstring(profile.NetworkAdapter(), 2));
        }
        catch (hresult_error const& ex)
        {
            profileString.append(exception_to_wstring(ex));
        }
        wprintf(L"%ws\n", profileString.c_str());
        profileString.clear();

        try
        {
            profileString.append(PrintHeader(L"WlanConnectionProfile"));
            profileString.append(tabs_to_wstring(1));
            profileString.append(L"IsWlanConnectionProfile: ").append(to_wstring(profile.IsWlanConnectionProfile())).append(L"\n");
            profileString.append(tabs_to_wstring(1));
            profileString.append(L"CanDelete: ").append(to_wstring(profile.CanDelete())).append(L"\n");

            if (profile.IsWlanConnectionProfile())
            {
                profileString.append(tabs_to_wstring(1));
                profileString.append(L"GetSignalBars: ").append(to_wstring(profile.GetSignalBars())).append(L"\n");

                const auto wlanConnectionProfileDetails = profile.WlanConnectionProfileDetails();
                profileString.append(tabs_to_wstring(1));
                profileString.append(L"WlanConnectionProfileDetails\n");
                profileString.append(tabs_to_wstring(2));
                profileString.append(L"ConnectedSsid: ").append(wlanConnectionProfileDetails.GetConnectedSsid()).append(L"\n");
            }
        }
        catch (hresult_error const& ex)
        {
            profileString.append(exception_to_wstring(ex));
        }
        wprintf(L"%ws\n", profileString.c_str());
        profileString.clear();

        try
        {
            profileString.append(PrintHeader(L"WwanConnectionProfile"));
            profileString.append(tabs_to_wstring(1));
            profileString.append(L"IsWwanConnectionProfile: ").append(to_wstring(profile.IsWwanConnectionProfile())).append(L"\n");
            profileString.append(tabs_to_wstring(1));
            profileString.append(L"CanDelete: ").append(to_wstring(profile.CanDelete())).append(L"\n");

            if (profile.IsWwanConnectionProfile())
            {
                profileString.append(tabs_to_wstring(1));
                profileString.append(L"GetSignalBars: ").append(to_wstring(profile.GetSignalBars())).append(L"\n");

                const auto wwanConnectionProfileDetails = profile.WwanConnectionProfileDetails();
                profileString.append(tabs_to_wstring(1));
                profileString.append(L"WwanConnectionProfileDetails\n");
                profileString.append(tabs_to_wstring(2));
                profileString.append(L"AccessPointName: ").append(wwanConnectionProfileDetails.AccessPointName()).append(L"\n");
                profileString.append(tabs_to_wstring(2));
                profileString.append(L"HomeProviderId: ").append(wwanConnectionProfileDetails.HomeProviderId()).append(L"\n");
                profileString.append(tabs_to_wstring(2));
                profileString.append(L"IPKind: ").append(to_wstring(wwanConnectionProfileDetails.IPKind())).append(L"\n");
                profileString.append(tabs_to_wstring(2));
                profileString.append(L"CurrentDataClass:").append(to_wstring(wwanConnectionProfileDetails.GetCurrentDataClass())).append(L"\n");
                profileString.append(tabs_to_wstring(2));
                profileString.append(L"NetworkRegistrationState:").append(to_wstring(wwanConnectionProfileDetails.GetNetworkRegistrationState())).append(L"\n");
                profileString.append(tabs_to_wstring(2));
                profileString.append(L"PurposeGuids\n");
                unsigned purposeGuidCount = 0;
                for (const auto guid : wwanConnectionProfileDetails.PurposeGuids())
                {
                    ++purposeGuidCount;
                    profileString.append(tabs_to_wstring(3));
                    profileString.append(to_wstring(purposeGuidCount)).append(L". ").append(to_wstring(guid)).append(L"\n");
                }
            }
        }
        catch (hresult_error const& ex)
        {
            profileString.append(exception_to_wstring(ex));
        }
        wprintf(L"%ws\n", profileString.c_str());
        profileString.clear();

        NetworkUsageStates usageStates{};
        usageStates.Shared = TriStates::DoNotCare;
        usageStates.Roaming = TriStates::DoNotCare;
        const auto duration = clock::duration(std::chrono::hours(24));
        const auto now = clock::now();

        try
        {
            profileString.append(PrintHeader(L"GetNetworkUsageAsync (for the past 24 hours)"));
            unsigned usageCount = 0;
            for (const auto& usage : profile.GetNetworkUsageAsync(now - duration, now, DataUsageGranularity::Total, usageStates).get())
            {
                ++usageCount;
                profileString.append(tabs_to_wstring(1));
                profileString.append(to_wstring(usageCount)).append(L". ConnectionDuration: ").append(to_wstring(usage.ConnectionDuration())).append(L"\n");
                profileString.append(tabs_to_wstring(2));
                profileString.append(L"BytesSent: ").append(std::to_wstring(usage.BytesSent())).append(L"\n");
                profileString.append(tabs_to_wstring(2));
                profileString.append(L"BytesReceived: ").append(std::to_wstring(usage.BytesReceived())).append(L"\n");
            }
        }
        catch (hresult_error const& ex)
        {
            profileString.append(exception_to_wstring(ex));
        }
        wprintf(L"%ws\n", profileString.c_str());
        profileString.clear();

        try
        {
            profileString.append(PrintHeader(L"GetProviderNetworkUsageAsync (for the past 24 hours)"));
            unsigned networkUsageCount = 0;
            for (const auto& usage : profile.GetProviderNetworkUsageAsync(now - duration, now, usageStates).get())
            {
                ++networkUsageCount;
                profileString.append(tabs_to_wstring(1));
                profileString.append(to_wstring(networkUsageCount)).append(L". ProviderId: ").append(usage.ProviderId()).append(L"\n");
                profileString.append(tabs_to_wstring(2));
                profileString.append(L"BytesSent: ").append(std::to_wstring(usage.BytesSent())).append(L"\n");
                profileString.append(tabs_to_wstring(2));
                profileString.append(L"BytesReceived: ").append(std::to_wstring(usage.BytesReceived())).append(L"\n");
            }
        }
        catch (hresult_error const& ex)
        {
            profileString.append(exception_to_wstring(ex));
        }
        wprintf(L"%ws\n", profileString.c_str());
        profileString.clear();

        try
        {
            profileString.append(PrintHeader(L"GetConnectivityIntervalsAsync (for the past 24 hours)"));
            unsigned connectivityIntervalCount = 0;
            for (const auto& usage : profile.GetConnectivityIntervalsAsync(now - duration, now, usageStates).get())
            {
                ++connectivityIntervalCount;
                profileString.append(tabs_to_wstring(1));
                profileString.append(std::to_wstring(connectivityIntervalCount)).append(L". ConnectionDuration: ").append(to_wstring(usage.ConnectionDuration())).append(L"\n");
                profileString.append(tabs_to_wstring(2));
                profileString.append(L"StartTime: ").append(to_wstring(usage.StartTime())).append(L"\n");
            }
        }
        catch (hresult_error const& ex)
        {
            profileString.append(exception_to_wstring(ex));
        }
        wprintf(L"%ws\n", profileString.c_str());
        profileString.clear();

        try
        {
            profileString.append(PrintHeader(L"GetAttributedNetworkUsageAsync (for the past 24 hours)"));
            unsigned attributedNetworkUsageCount = 0;
            for (const auto& usage : profile.GetAttributedNetworkUsageAsync(now - duration, now, usageStates).get())
            {
                ++attributedNetworkUsageCount;
                profileString.append(tabs_to_wstring(1));
                profileString.append(to_wstring(attributedNetworkUsageCount)).append(L". AttributionId: ").append(usage.AttributionId()).append(L"\n");
                profileString.append(tabs_to_wstring(2));
                profileString.append(L"BytesReceived: ").append(to_wstring(usage.BytesReceived())).append(L"\n");
                profileString.append(tabs_to_wstring(2));
                profileString.append(L"BytesSent: ").append(to_wstring(usage.BytesSent())).append(L"\n");
            }
        }
        catch (hresult_error const& ex)
        {
            profileString.append(exception_to_wstring(ex));
        }
        wprintf(L"%ws\n\n", profileString.c_str());
    }
}
catch (hresult_error const& ex)
{
    wprintf(L"%ws\n", exception_to_wstring(ex).c_str());
}
catch (...)
{
    const auto hr = wil::ResultFromCaughtException();
    wprintf(L"exception thrown: 0x%x\n", hr);
}
