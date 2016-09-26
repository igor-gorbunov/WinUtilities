#include "DriverInstallUtility.h"

HRESULT DrvInstall_Create(IN LPCTSTR ServiceName, IN OPTIONAL LPCTSTR DisplayName, IN LPCTSTR DriverFilePath)
{
	SC_HANDLE ScManager, Service;

	if (NULL == DisplayName)
		DisplayName = ServiceName;

	if (NULL == (ScManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE)))
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, GetLastError());

	if (NULL == (Service = CreateService(ScManager, ServiceName, DisplayName, 0, SERVICE_KERNEL_DRIVER,
		SERVICE_SYSTEM_START, SERVICE_ERROR_NORMAL, DriverFilePath, NULL, NULL, NULL, NULL, NULL)))
	{
		DWORD Result = GetLastError();

		if (FALSE == CloseServiceHandle(ScManager))
			OutputDebugString(_T("DrvInstall_Create(): Call to CloseServiceHandle() failed.\n"));

		if (ERROR_SERVICE_EXISTS == Result)
			return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, Result);
		else
			return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, Result);
	}
	if (FALSE == CloseServiceHandle(Service))
		OutputDebugString(_T("DrvInstall_Create(): Call to CloseServiceHandle() failed.\n"));
	if (FALSE == CloseServiceHandle(ScManager))
		OutputDebugString(_T("DrvInstall_Create(): Call to CloseServiceHandle() failed.\n"));

	return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, ERROR_SUCCESS);
}

HRESULT DrvInstall_Start(IN LPCTSTR DriverName)
{
	DWORD LastError;
	SC_HANDLE ScManager, Service;
	DWORD NumberOfBytesNeeded;
	SERVICE_STATUS_PROCESS ServiceStatus;
	DWORD OldCheckPoint;

	HRESULT Result = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, ERROR_UNIDENTIFIED_ERROR);

	if (NULL == (ScManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)))
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, GetLastError());

	if (NULL == (Service = OpenService(ScManager, DriverName, SERVICE_START | SERVICE_QUERY_STATUS)))
	{
		LastError = GetLastError();

		if (FALSE == CloseServiceHandle(ScManager))
			OutputDebugString(_T("DrvInstall_Start(): Call to CloseServiceHandle() failed.\n"));

		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, LastError);
	}

	if (FALSE == StartService(Service, 0, NULL))
	{
		LastError = GetLastError();

		if (FALSE == CloseServiceHandle(Service))
			OutputDebugString(_T("DrvInstall_Start(): Call to CloseServiceHandle() failed.\n"));
		if (FALSE == CloseServiceHandle(ScManager))
			OutputDebugString(_T("DrvInstall_Start(): Call to CloseServiceHandle() failed.\n"));

		if (ERROR_SERVICE_ALREADY_RUNNING == LastError)
			return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, LastError);
		else
			return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, LastError);
	}

	if (FALSE == QueryServiceStatusEx(Service, SC_STATUS_PROCESS_INFO, (LPBYTE)&ServiceStatus,
		sizeof(ServiceStatus), &NumberOfBytesNeeded))
	{
		LastError = GetLastError();

		if (FALSE == CloseServiceHandle(Service))
			OutputDebugString(_T("DrvInstall_Start(): Call to CloseServiceHandle() failed.\n"));
		if (FALSE == CloseServiceHandle(ScManager))
			OutputDebugString(_T("DrvInstall_Start(): Call to CloseServiceHandle() failed.\n"));

		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, LastError);
	}

	while (SERVICE_START_PENDING == ServiceStatus.dwCurrentState)
	{
		DWORD TotalWaitTime = 0, WaitTime;
		WaitTime = (0 < ServiceStatus.dwWaitHint && 1000 > ServiceStatus.dwWaitHint) ? ServiceStatus.dwWaitHint : 500;
		Sleep(WaitTime);
		TotalWaitTime += WaitTime;

		OldCheckPoint = ServiceStatus.dwCheckPoint;

		if (FALSE == QueryServiceStatusEx(Service, SC_STATUS_PROCESS_INFO, (LPBYTE)&ServiceStatus,
			sizeof(ServiceStatus), &NumberOfBytesNeeded))
		{
			LastError = GetLastError();

			if (FALSE == CloseServiceHandle(Service))
				OutputDebugString(_T("DrvInstall_Start(): Call to CloseServiceHandle() failed.\n"));
			if (FALSE == CloseServiceHandle(ScManager))
				OutputDebugString(_T("DrvInstall_Start(): Call to CloseServiceHandle() failed.\n"));

			return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, LastError);
		}

		if (ServiceStatus.dwCheckPoint == OldCheckPoint && TotalWaitTime > ServiceStatus.dwWaitHint)
		{
			Result = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, ERROR_SERVICE_REQUEST_TIMEOUT);
			break;
		}
	}

	if (SERVICE_RUNNING == ServiceStatus.dwCurrentState)
		Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, ERROR_SUCCESS);

	if (FALSE == CloseServiceHandle(Service))
		OutputDebugString(_T("DrvInstall_Start(): Call to CloseServiceHandle() failed.\n"));
	if (FALSE == CloseServiceHandle(ScManager))
		OutputDebugString(_T("DrvInstall_Start(): Call to CloseServiceHandle() failed.\n"));

	return Result;
}

HRESULT DrvInstall_Stop(IN LPCTSTR DriverName)
{
	DWORD LastError;
	SC_HANDLE ScManager, Service;
	SERVICE_STATUS ServiceStatus;

	HRESULT Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, ERROR_SUCCESS);

	if (NULL == (ScManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)))
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, GetLastError());

	if (NULL == (Service = OpenService(ScManager, DriverName, SERVICE_STOP | SERVICE_QUERY_STATUS)))
	{
		LastError = GetLastError();

		if (FALSE == CloseServiceHandle(ScManager))
			OutputDebugString(_T("DrvInstall_Stop(): Call to CloseServiceHandle() failed.\n"));

		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, LastError);
	}

	if (FALSE == ControlService(Service, SERVICE_CONTROL_STOP, &ServiceStatus))
	{
		LastError = GetLastError();
		if (ERROR_SERVICE_NOT_ACTIVE == LastError)
			Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, LastError);
		else
			Result = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, LastError);
	}

	if (FALSE == CloseServiceHandle(Service))
		OutputDebugString(_T("DrvInstall_Stop(): Call to CloseServiceHandle() failed.\n"));
	if (FALSE == CloseServiceHandle(ScManager))
		OutputDebugString(_T("DrvInstall_Stop(): Call to CloseServiceHandle() failed.\n"));

	return Result;
}

HRESULT DrvInstall_Remove(IN LPCTSTR DriverName)
{
	DWORD LastError;
	SC_HANDLE ScManager, Service;

	HRESULT Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, ERROR_SUCCESS);

	if (NULL == (ScManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)))
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, GetLastError());

	if (NULL == (Service = OpenService(ScManager, DriverName, DELETE)))
	{
		LastError = GetLastError();

		if (FALSE == CloseServiceHandle(ScManager))
			OutputDebugString(_T("DrvInstall_Remove(): Call to CloseServiceHandle() failed.\n"));

		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, LastError);
	}

	if (FALSE == DeleteService(Service))
	{
		LastError = GetLastError();

		if (ERROR_SERVICE_MARKED_FOR_DELETE == LastError)
			Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, LastError);
		else
			Result = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_DEPLOYMENT_SERVICES_MANAGEMENT, LastError);
	}

	if (FALSE == CloseServiceHandle(Service))
		OutputDebugString(_T("DrvInstall_Remove(): Call to CloseServiceHandle() failed.\n"));
	if (FALSE == CloseServiceHandle(ScManager))
		OutputDebugString(_T("DrvInstall_Remove(): Call to CloseServiceHandle() failed.\n"));

	return Result;
}
